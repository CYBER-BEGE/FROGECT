// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFrogPlayerCharacter::AFrogPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CharacterMovement 세팅
	GetCharacterMovement()->BrakingDecelerationFalling = 50.0f; // 공중 감속
	GetCharacterMovement()->AirControl = 0.7f;					// 공중 제어
	GetCharacterMovement()->GravityScale = 2.0f;				// 중력 배율
}

// Called when the game starts or when spawned
void AFrogPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed *= MoveSpeed;			// 이동 속도
	GetCharacterMovement()->JumpZVelocity *= JumpPower;			// 점프 힘
}

// Called every frame
void AFrogPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}

void AFrogPlayerCharacter::DoCrouchEnd()
{
	UnCrouch();
}