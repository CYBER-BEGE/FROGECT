// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogDamageComponent.h"
#include "FrogHealthComponent.h"
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
}


// Called every frame
void UFrogDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UFrogDamageComponent::ApplyDamage(AActor* AttackActor, AActor* TargetActor, float AttackPower)
{
	if (!TargetActor || TargetActor == AttackActor) return; 
	
	// HealthComponent 찾기 (HealthComponent 내부 함수로 변경, 이런식으로 갖고오지 마셈)
	if (UFrogHealthComponent* HealthComponent = TargetActor->FindComponentByClass<UFrogHealthComponent>())
	{
		HealthComponent->DamageTaken(AttackActor, AttackPower);
	}
}