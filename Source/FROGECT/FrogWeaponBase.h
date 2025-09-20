// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrogDamageComponent.h"
#include "FrogWeaponBase.generated.h"

UCLASS()
class FROGECT_API AFrogWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFrogWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UStaticMeshComponent* GetMesh() const { return WeaponMesh; }
	UFrogDamageComponent* GetFrogDamageComponent() const { return FrogDamageComponent; }

protected:
	UPROPERTY(EditAnywhere, Category = "State")
	float AttackPower = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UShapeComponent* WeaponCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UFrogDamageComponent* FrogDamageComponent;
};
