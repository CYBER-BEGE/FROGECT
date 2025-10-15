// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FrogProjectile.h"
#include "FrogGrapplingHook.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFrogPlayerCharacter::AFrogPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HookSpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hook Spawn Point"));
	HookSpawnPoint->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AFrogPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

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
}

// Called every frame
void AFrogPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *GetCharacterMovement()->Velocity.ToString());

	GrapplePull();
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

		// Hook
		EnhancedInputComponent->BindAction(HookAction, ETriggerEvent::Started, this, &AFrogPlayerCharacter::DoHookStart);
		//EnhancedInputComponent->BindAction(HookAction, ETriggerEvent::Completed, this, &AFrogPlayerCharacter::DoHookEnd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Enhanced Input Component!"));
	}
}

void AFrogPlayerCharacter::ResetMovementComps()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f * MoveSpeedScale;	// 속도
	GetCharacterMovement()->GravityScale = 2.0f;					// 중력
	GetCharacterMovement()->GroundFriction = 8.0f;					// 마찰력
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;	// 감속력
	GetCharacterMovement()->BrakingDecelerationFalling = 50.0f;		// 공중 감속력
	GetCharacterMovement()->AirControl = 0.7f;						// 공중 제어
}

void AFrogPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (!bCanDash && !bIsDashing) // 대시 가능/대시 중이 아닐 시
	{
		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AFrogPlayerCharacter::DashCooldown, 0.5f, false);
	}
}

void AFrogPlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();

	DoMove(MovementVector.X, MovementVector.Y);
}

void AFrogPlayerCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		AddMovementInput(GetActorRightVector(), Right * MoveSpeedScale);
		AddMovementInput(GetActorForwardVector(), Forward * MoveSpeedScale);
	}
}

void AFrogPlayerCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
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

	DoHookEnd(); // 점프 시 그래플링 훅 해제
}

void AFrogPlayerCharacter::DoJumpEnd()
{
	StopJumping();
}

void AFrogPlayerCharacter::DoCrouchStart()
{
	if (bIsHookAttaching) return; // 그래플링 중일 시 종료

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
	//if (!HasJetpack) return;					// 제트팩 없을 시 종료
	if (!bCanDash || bIsDashing) return;		// 대시 불가능/대시 중일 시 종료
	if (bIsHookAttaching) return;				// 그래플링 중일 시 종료
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

void AFrogPlayerCharacter::DoHookStart()
{
	//if (!HasHook) return;
	if (!bCanGrapple || bIsHookAttaching) return; // 이미 그래플링 중일 시 종료

	bCanGrapple = false;

	/* 훅 발사 위치 및 방향 설정 */
	FRotator ControlRot = Controller->GetControlRotation();
	FVector Start = GetActorLocation() + FVector(0.f, 0.f, BaseEyeHeight) - -GetActorRightVector() * 10.f;; // 캐릭터 눈 높이
	FVector End = Start + ControlRot.Vector() * 5000.f; // 시야 방향으로 10,000 유닛 쏘기

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		End = Hit.Location; // 실제 맞은 위치
	}

	FRotator Rotation = (End - Start).Rotation();
	FVector Location = HookSpawnPoint->GetComponentLocation();
	
	// 훅 투사체 스폰
	GrapplingHookInstance = GetWorld()->SpawnActor<AFrogGrapplingHook>(GrapplingHookClass, Location, Rotation);

	/* 케이블 연결 */
	if (GrapplingHookInstance)
	{
		GrapplingHookInstance->SetOwner(this); // 소유자 설정

		// 케이블 시작점 → 캐릭터의 HookSpawnPoint
		GrapplingHookInstance->HookCable->AttachToComponent(HookSpawnPoint, FAttachmentTransformRules::KeepRelativeTransform);
		GrapplingHookInstance->HookCable->bAttachStart = true;

		// 케이블 끝점 → GrapplingHookInstance (Projectile)
		GrapplingHookInstance->HookCable->SetAttachEndToComponent(GrapplingHookInstance->GetRootComponent());
		GrapplingHookInstance->HookCable->bAttachEnd = true;
	}

	// 일정 시간 후 훅 발사 종료
	GetWorldTimerManager().SetTimer(HookTimerHandle, this, &AFrogPlayerCharacter::DoHookEnd, 0.4f, false);
}

void AFrogPlayerCharacter::DoHookEnd()
{
	if (GrapplingHookInstance && GrapplingHookInstance->IsValidLowLevel())
	{
		GrapplingHookInstance->DestroyProjectile();
		GrapplingHookInstance = nullptr; // 참조 정리
	}

	bCanGrapple = true;
	bIsHookAttaching = false;
	ResetMovementComps();
}

void AFrogPlayerCharacter::OnHookAttached(const FVector& Target)
{
	bIsHookAttaching = true;

	GetWorldTimerManager().ClearTimer(HookTimerHandle);

	HookTargetLocation = Target;
	HookTargetLocation.Z += 50.0f; // 약간 타겟 위로 보정

	LaunchCharacter(FVector(0, 0, 200.0f), false, false); // 그래플링 직전 공중으로 약간 띄워주기

	GetCharacterMovement()->GravityScale = 0.0f;				// 중력 0
	GetCharacterMovement()->GroundFriction = 0.0f;				// 마찰력 0
	GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;	// 감속력 0

	GetCharacterMovement()->BrakingDecelerationFalling = 100.0f;// 공중 감속력 상승
	GetCharacterMovement()->AirControl = 0.6f;					// 공중 제어 하락
}

void AFrogPlayerCharacter::GrapplePull()
{
	if (!bIsHookAttaching) return; // 그래플링 중이 아닐 시 종료

	/* force 기반 이동 */
	FVector PullDir = (HookTargetLocation - GetActorLocation()).GetSafeNormal();	// 훅 타겟 방향 벡터
	float PullPower = GetCharacterMovement()->Mass * GrapplePullPower;				// 힘 = 질량 * 가속도
	GetCharacterMovement()->AddForce(PullDir * PullPower);

	/* 도착 체크 (100 단위 거리 이내) */
	FVector CurrentLocation = GetActorLocation();
	if (FVector::Dist(CurrentLocation, HookTargetLocation) < 100.f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Grappling has arrived!"));
		DoHookEnd();
		return;
	}

	/* 화면 벗어남 체크 */
	if (PlayerController)
	{
		FVector2D ScreenLocation;
		if (UGameplayStatics::ProjectWorldToScreen(PlayerController, HookTargetLocation, ScreenLocation)) // 3D 월드 좌표 → 2D 스크린 좌표 변환
		{
			int32 ScreenX, ScreenY;
			PlayerController->GetViewportSize(ScreenX, ScreenY); // 뷰포트 크기 가져오기

			// 스크린 좌표가 뷰포트 내에 있는지 체크
			bool bOnScreen =
				ScreenLocation.X >= 0 && ScreenLocation.X <= ScreenX &&
				ScreenLocation.Y >= 0 && ScreenLocation.Y <= ScreenY;

			if (!bOnScreen) // 화면 벗어남
			{
				//UE_LOG(LogTemp, Warning, TEXT("HookTarget out of view!"));
				DoHookEnd();
				return;
			}
		}
	}
}
