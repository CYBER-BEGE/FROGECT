// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UFrogDamageComponent* FrogDamageComponent;

	void test();

protected:
	UPROPERTY(EditAnywhere, Category = "State")
	float AttackPower = 10.0f;
};
