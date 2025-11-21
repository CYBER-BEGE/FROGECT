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
	/* Input Action */
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
	class UInputAction* HookAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* LickAction;

	/* Item */
	UPROPERTY(EditAnywhere, Category = "Gear")
	bool HasSword = false;

	UPROPERTY(EditAnywhere, Category = "Gear")
	bool HasJetpack = false;

	UPROPERTY(EditAnywhere, Category = "Gear")
	bool HasHook = false;

	/* Player Controller */
	UPROPERTY()
	APlayerController* PlayerController;

	/* Movement Components */
	void ResetMovementComps();

	/* Move */
	FVector2D MovementVector; // 인풋 받은 이동 벡터
	void MoveInput(const struct FInputActionValue& Value);
	void DoMove(float Right, float Forward);

	/* Look */
	void LookInput(const struct FInputActionValue& Value);
	void DoLook(float Yaw, float Pitch);

	/* Jump */
	void DoJumpStart();
	void DoJumpEnd();

	/* Crouch */
	void DoCrouchStart();
	void DoCrouchEnd();

	/* Dash */
	FTimerHandle DashTimerHandle;	// 대시 쿨타임 타이머 핸들
	bool bIsDashing = false;		// 대시 중인지 여부
	bool bCanDash = true;			// 대시 가능 여부

	void DoDashStart();
	void DoDashEnd();
	void DashCooldown();			// 대시 쿨타임 완료 함수

	/* Grappling Hook */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gear", meta = (AllowPrivateAccess = "true"))
	USceneComponent* HookSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Gear")
	TSubclassOf<class AFrogGrapplingHook> GrapplingHookClass;

	UPROPERTY()
	AFrogGrapplingHook* GrapplingHookInstance; // 그래플링 훅 투사체 인스턴스

	UPROPERTY(EditAnywhere, Category = "Gear")
	float GrapplePullPower = 2000.0f;

	FVector HookTargetLocation;		// 그래플링 훅이 붙은 위치
	FTimerHandle HookTimerHandle;	// 그래플링 훅 발사 지속 타이머 핸들

	bool bCanGrapple = true;
	bool bIsHookAttaching = false;

	void DoHookStart();
	void DoHookEnd();
	void GrapplePull(); // 그래플링 훅에 당겨지는 함수

	/* Attack */
	FTimerHandle AttackTimerHandle;
	bool bCanAttack = true;

	// 임시 손 소켓 - 에셋 추가 후 삭제
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gear", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RightHand;

	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<class AFrogWeaponBase> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = "Gear")
	class AFrogWeaponBase* WeaponInstance;

	void DoAttackStart();
	void DoAttackEnd();

	/* Tounge */
	bool bCanToungeLick = true;
	bool bCanToungeEat = false;
	bool bCanToungeGrapple = false;
	bool bIsToungeAttaching = false;
	FTimerHandle ToungeTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gear", meta = (AllowPrivateAccess = "true"))
	USceneComponent* TongueSpawnPoint;

	void DoToungeLickStart();
	void DoToungeLickEnd();

	void DoToungeEat(AActor& Target);
	void DoToungeGrapple();

protected:
	void Landed(const FHitResult& Hit) override;

public:
	void OnHookAttached(const FVector& Target); // 그래플링 훅이 물체에 붙었을 때 호출되는 함수

	void OnToungeAttached(AActor& Target);

	UFUNCTION()
	void OnToungeReturned();

	UPROPERTY()
	AActor* PendingEdibleActor = nullptr;

	UPROPERTY()
	TSubclassOf<AActor> StoredObjectClass = nullptr;

};
