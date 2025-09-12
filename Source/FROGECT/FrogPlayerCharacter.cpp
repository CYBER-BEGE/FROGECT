// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "FrogWeaponBase.h"
#include "FrogDamageComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFrogPlayerCharacter::AFrogPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFrogPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// CharacterMovement 세팅
	GetCharacterMovement()->BrakingDecelerationFalling = 50.0f;				// 공중 감속력
	GetCharacterMovement()->AirControl = 0.7f;								// 공중 제어
	GetCharacterMovement()->GravityScale = 2.0f;							// 중력 배율

	GetCharacterMovement()->GroundFriction = 8.0f;							// 마찰력
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;			// 감속력

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;	// 웅크리기 가능
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;			// 웅크리고 턱을 내려가기 가능

	GetCharacterMovement()->MaxWalkSpeed *= MoveSpeedScale;					// 이동 속도
	GetCharacterMovement()->JumpZVelocity *= JumpPowerScale;				// 점프 힘

	// Weapon 스폰 및 어태치
	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<AFrogWeaponBase>(WeaponClass);
		if (Weapon)
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon"));
			Weapon->SetOwner(this);
		}
		
		// 임시: 플레이어 에셋 생기면 소켓으로 변경
		Weapon->GetRootComponent()->SetRelativeLocation(FVector(20, 40, 0));
		Weapon->GetRootComponent()->SetRelativeRotation(FRotator(-40, 0, 0));
	}
}

// Called every frame
void AFrogPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *GetCharacterMovement()->Velocity.ToString());
}

// Called to bind functionality to input
void AFrogPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrogPlayerCharacter::MoveInput);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrogPlayerCharacter::LookInput);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFrogPlayerCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFrogPlayerCharacter::DoJumpEnd);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFrogPlayerCharacter::DoCrouchStart);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFrogPlayerCharacter::DoCrouchEnd);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AFrogPlayerCharacter::DoDashStart);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AFrogPlayerCharacter::DoAttackStart);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Enhanced Input Component!"));
	}
}

void AFrogPlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();

	DoMove(MovementVector.X, MovementVector.Y);
}

void AFrogPlayerCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AFrogPlayerCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		AddMovementInput(GetActorRightVector(), Right * MoveSpeedScale);
		AddMovementInput(GetActorForwardVector(), Forward * MoveSpeedScale);
	}
}

void AFrogPlayerCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AFrogPlayerCharacter::DoJumpStart()
{
	Jump();
}

void AFrogPlayerCharacter::DoJumpEnd()
{
	StopJumping();
}

void AFrogPlayerCharacter::DoCrouchStart()
{
	Crouch();
	
	if (!GetCharacterMovement()->Velocity.IsNearlyZero() && !GetCharacterMovement()->IsFalling()) // 정지/공중이 아닐 시 슬라이딩
	{
		FVector SlideDir = GetCharacterMovement()->Velocity.GetSafeNormal2D(); // XY벡터에서 방향만 추출
		FVector SlideImpulse = SlideDir * 800.0f * MoveSpeedScale;

		LaunchCharacter(SlideImpulse, true, false); // 수평 방향으로만 임펄스 적용

		GetCharacterMovement()->GroundFriction = 0.0f; // 마찰력 0
		GetCharacterMovement()->BrakingDecelerationWalking = 466.0f * MoveSpeedScale; // 감속력 감소
	}
}

void AFrogPlayerCharacter::DoCrouchEnd()
{
	UnCrouch();

	ResetMovementComps(); // 본래 마찰력/감속력 복구
}

void AFrogPlayerCharacter::DoDashStart()
{
	if (!bCanDash || bIsDashing) return;		// 대시 불가능/대시 중일 시 종료
	if (MovementVector.IsNearlyZero()) return;	// 이동 입력이 없을 시 종료

	bIsDashing = true;
	bCanDash = false;

	FRotator CameraRot = Controller->GetControlRotation(); // 카메라 회전값
	FVector ForwardDir = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::X);
	FVector RightDir = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::Y);
	FVector DashDir = (ForwardDir * MovementVector.Y + RightDir * MovementVector.X).GetSafeNormal();

	LaunchCharacter(DashDir * 2000.0f, true, true); // 임펄스 적용

	GetCharacterMovement()->Velocity = FVector::ZeroVector;		// 이동 정지
	GetCharacterMovement()->GravityScale = 0.0f;				// 중력 0
	GetCharacterMovement()->GroundFriction = 0.0f;				// 마찰력 0
	GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;	// 감속력 0
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;				// 이동 속도 0

	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AFrogPlayerCharacter::DoDashEnd, 0.2f, false);
}

void AFrogPlayerCharacter::DoDashEnd()
{
	bIsDashing = false;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;	// 대시 정지
	ResetMovementComps(); // 본래 속도/중력/마찰력/감속력 복구

	if (!GetCharacterMovement()->IsFalling()) // 점프 중이 아닐 시
	{
		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AFrogPlayerCharacter::DashCooldown, 0.5f, false);
	}
}

void AFrogPlayerCharacter::DashCooldown()
{
	bCanDash = true;
}

void AFrogPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (!bCanDash && !bIsDashing) // 대시 가능/대시 중이 아닐 시
	{
		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AFrogPlayerCharacter::DashCooldown, 0.5f, false);
	}
}

void AFrogPlayerCharacter::ResetMovementComps()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f * MoveSpeedScale;	// 속도
	GetCharacterMovement()->GravityScale = 2.0f;					// 중력
	GetCharacterMovement()->GroundFriction = 8.0f;					// 마찰력
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;	// 감속력
}

void AFrogPlayerCharacter::DoAttackStart()
{
	if (!HasSword) return;
	//공격 가능상태인지 확인하는 bool 변수 체크

	UE_LOG(LogTemp, Display, TEXT("Has SWORD, LEFT MOUSE CLICK"));

	//AttackComponent에서 콜라이더 ON 함수 호출
	Weapon->GetFrogDamageComponent()->EnableCollision();

	//AttackComp의 콜리전 결과 받기
	//AttackPower 가져오기
	//HealthCompoent에서 DamageTaken 실행

	//딜레이 후 DoAtkEnd 호출
}

void AFrogPlayerCharacter::DoAttackEnd()
{
	//AttackComponent에서 콜라이더 OFF 함수 호출
	//bool 변수를 만들고 다시 공격 가능 상태로 만들기
}
