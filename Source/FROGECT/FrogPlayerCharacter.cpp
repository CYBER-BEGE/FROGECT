// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogPlayerCharacter.h"
#include "EnhancedInputComponent.h"
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

	GetCharacterMovement()->MaxWalkSpeed *= MoveSpeed;						// 이동 속도
	GetCharacterMovement()->JumpZVelocity *= JumpPower;						// 점프 힘
}

// Called every frame
void AFrogPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *GetCharacterMovement()->Velocity.ToString());
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
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Enhanced Input Component!"));
	}
}

void AFrogPlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

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
		AddMovementInput(GetActorRightVector(), Right * MoveSpeed);
		AddMovementInput(GetActorForwardVector(), Forward * MoveSpeed);
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
		FVector SlideImpulse = SlideDir * 800.0f * MoveSpeed;
		LaunchCharacter(SlideImpulse, true, false); // 수평 방향으로만 임펄스 적용

		GetCharacterMovement()->GroundFriction = 0.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 466.0f * MoveSpeed;
	}
}

void AFrogPlayerCharacter::DoCrouchEnd()
{
	UnCrouch();

	// 본래 마찰력/감속력 복구
	GetCharacterMovement()->GroundFriction = 8.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
}
