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

	void DoMove(float Right, float Forward);
	void DoLook(float Yaw, float Pitch);

	void DoJumpStart();
	void DoJumpEnd();

	void DoCrouchStart();
	void DoCrouchEnd();

	void DoDashStart();
	void DoDashEnd();
	void DashCooldown();

	void ResetMovementComps();

	FVector2D MovementVector;		// 인풋 받은 이동 벡터

	FTimerHandle DashTimerHandle;	// 대시 타이머 핸들
	bool bIsDashing = false;		// 대시 중인지 여부
	bool bCanDash = true;			// 대시 가능 여부

	class UFrogDamageComponent* DamageComponent;

	void DoAttackStart();
	void DoAttackEnd();

protected:
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* AttackAction;

	void Landed(const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	
	TSubclassOf<class AFrogWeaponBase> WeaponClass;
	class AFrogWeaponBase* Weapon;

};
