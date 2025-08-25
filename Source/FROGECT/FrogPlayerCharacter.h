// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogCharacterBase.h"
#include "FrogPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FROGECT_API AFrogPlayerCharacter : public AFrogCharacterBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AFrogPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "HasItem")
	bool HasSword = false;

	UPROPERTY(EditAnywhere, Category = "HasItem")
	bool HasJetpack = false;

	UPROPERTY(EditAnywhere, Category = "HasItem")
	bool HasHook = false;

	void MoveInput(const struct FInputActionValue& Value);

	void LookInput(const struct FInputActionValue& Value);

	void DoLook(float Yaw, float Pitch);

	void DoMove(float Right, float Forward);

	void DoJumpStart();

	void DoJumpEnd();

protected:
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* JumpAction;

};
