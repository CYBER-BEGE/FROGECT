// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogCharacterBase.h"
#include "FrogAIController.h"
#include "FrogEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FROGECT_API AFrogEnemyCharacter : public AFrogCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFrogEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};
