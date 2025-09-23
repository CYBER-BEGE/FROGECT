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
	if (!TargetActor || TargetActor == AttackActor) return; // 피격자가 nullptr, 공격자 본인이면 종료
	
	UFrogHealthComponent* HealthComponent = TargetActor->FindComponentByClass<UFrogHealthComponent>(); // 피격자의 HealthComponent 받아오기
	
	if (HealthComponent)
	{
		HealthComponent->DamageTaken(AttackActor, AttackPower);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s HealthComponent 없음"), *TargetActor->GetActorLabel());
	}
}