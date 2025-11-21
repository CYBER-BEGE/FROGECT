// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/DelegateCombinations.h" 
#include "FrogProjectileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileReturned);

UCLASS()
class FROGECT_API AFrogProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFrogProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComponent;

public:
	void DestroyProjectile();

	void BounceProjectile();

	void ReturnProjectile();

	UPROPERTY(EditAnywhere)
	FOnProjectileReturned OnProjectileReturned;

private:
	/** Tick에서 실제 위치 갱신 처리 */
	void HandleReturn(float DeltaTime);

	/** Return 속도 */
	UPROPERTY(EditAnywhere, Category = "Return")
	float ReturnSpeed = 5000.f;

	/** Return 중인지 체크 */
	bool bReturning = false;
};
