// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogGrapplingHook.h"

// Sets default values
AFrogGrapplingHook::AFrogGrapplingHook()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HookCable = CreateDefaultSubobject<UCableComponent>(TEXT("HookCable"));
	HookCable->SetupAttachment(RootComponent);

	HookCable->EndLocation = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AFrogGrapplingHook::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFrogGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}