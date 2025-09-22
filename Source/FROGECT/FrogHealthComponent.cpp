// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogHealthComponent.h"

// Sets default values for this component's properties
UFrogHealthComponent::UFrogHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Health = GetOwner() -> GetMaxHealth(); // GetMaxHealth() 함수는 없음
	
}


// Called when the game starts
void UFrogHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFrogHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFrogHealthComponent::DamageTaken(AActor* AttackActor, float AttackPower)
{
	if (AttackPower <= 0.f || Health <= 0.f) return;
	
	Health -= AttackPower;
	UE_LOG(LogTemp, Warning, TEXT("%s 가 %s 공격"), *AttackActor->GetActorLabel(), *GetOwner()->GetActorLabel());
	UE_LOG(LogTemp, Warning, TEXT("%s 의 현재 HP: %f"), *GetOwner()->GetActorLabel(), Health);

	if (Health <= 0.f) 
	{
		// 사망, 파괴
		UE_LOG(LogTemp, Warning, TEXT("%s 가 %s 처치/파괴"), *AttackActor->GetActorLabel(), *GetOwner()->GetActorLabel());
	}
}

void UFrogHealthComponent::Heal()
{
}

