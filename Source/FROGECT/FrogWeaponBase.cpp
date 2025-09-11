// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogWeaponBase.h"
#include "FrogDamageComponent.h"

// Sets default values
AFrogWeaponBase::AFrogWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FrogDamageComponent = CreateDefaultSubobject<UFrogDamageComponent>(TEXT("Damage Component"));
	FrogDamageComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFrogWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFrogWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

