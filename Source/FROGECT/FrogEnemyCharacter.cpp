// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogEnemyCharacter.h"

// Sets default values
AFrogEnemyCharacter::AFrogEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 이 캐릭터를 어떤 AI 컨트롤러가 잡을지 지정
	AIControllerClass = AFrogAIController::StaticClass();
}

// Called when the game starts or when spawned
void AFrogEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

}
