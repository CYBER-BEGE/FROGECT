// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogProjectileBase.h"

#include "CableComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "FrogGrapplingHook.generated.h"

/**
 * 
 */
UCLASS()
class FROGECT_API AFrogGrapplingHook : public AFrogProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AFrogGrapplingHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hook")
	UCableComponent* HookCable;

	void ReturnProjectile();

	UPROPERTY(EditAnywhere)
	FOnProjectileReturned OnProjectileReturned;

protected:
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	// Tick에서 실제 위치 변경
	void ToungeReturn(float DeltaTime);

	/** Return 속도 */
	UPROPERTY(EditAnywhere, Category = "Return")
	float ReturnSpeed = 5000.f;

	/** Return 중인지 체크 */
	bool bToungeReturning = false;
};
