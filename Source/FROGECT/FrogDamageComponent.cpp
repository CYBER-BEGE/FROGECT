// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogDamageComponent.h"
#include "FrogWeaponBase.h"

// Sets default values for this component's properties
UFrogDamageComponent::UFrogDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFrogDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AFrogWeaponBase* Owner = Cast<AFrogWeaponBase>(GetOwner());
	Owner->SetActorEnableCollision(false);
}


// Called every frame
void UFrogDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
