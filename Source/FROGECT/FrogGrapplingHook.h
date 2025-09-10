// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogProjectile.h"
#include "CableComponent.h"
#include "FrogGrapplingHook.generated.h"

/**
 * 
 */
UCLASS()
class FROGECT_API AFrogGrapplingHook : public AFrogProjectile
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
};
