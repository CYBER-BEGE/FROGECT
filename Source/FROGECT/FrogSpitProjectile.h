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
	UStaticMeshComponent* Mesh;
	UPrimitiveComponent* Collider;
	FVector Scale;

public:
	void GetEdibleActorData(class UFrogEdibleActorComponent* EdibleActorComponent);

};
