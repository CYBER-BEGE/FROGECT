//// Fill out your copyright notice in the Description page of Project Settings.

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
	void EnableWeaponOverlap();
	void DisableWeaponOverlap();

protected:
	UPROPERTY(EditAnywhere, Category = "State")
	float AttackPower = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UShapeComponent* WeaponCollider;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UFrogDamageComponent* FrogDamageComponent;

private:
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* ThisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
