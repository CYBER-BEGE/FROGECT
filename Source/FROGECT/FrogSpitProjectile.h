// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogProjectileBase.h"
#include "FrogSpitProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FROGECT_API AFrogSpitProjectile : public AFrogProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AFrogSpitProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UStaticMeshComponent* Mesh = nullptr;
	UPrimitiveComponent* Collider = nullptr;
	FVector Scale;

	FTimerHandle DestroyTimerHandle;
	bool bHasHit = false;

	void StartDestroyTimer();
	UFUNCTION()
	void OnSpitProjectileHit(const FHitResult& ImpactResult);

public:
	// Edible Actor 외형 값 복사
	void CloneEdibleActor(class UFrogEdibleActorComponent* EdibleActorComponent);

};
