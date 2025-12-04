// Fill out your copyright notice in the Description page of Project Settings.

#include "FrogEdibleActorComponent.h"

// Sets default values for this component's properties
UFrogEdibleActorComponent::UFrogEdibleActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UFrogEdibleActorComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheEdibleActorData();
}

// Called every frame
void UFrogEdibleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UFrogEdibleActorComponent::CacheEdibleActorData()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	CachedData.MeshData = Owner->FindComponentByClass<UStaticMeshComponent>(); // 메시
	CachedData.MaterialData = CachedData.MeshData->GetMaterial(0); // 머티리얼
	CachedData.ColliderData = Owner->FindComponentByClass<UPrimitiveComponent>(); // 콜라이더
	CachedData.ScaleData = Owner->GetActorScale3D(); // 크기
	
	// 공격타입(적 투사체라면 있을거임)
	// 그 외 추가되는 것들 반영(이펙트 파티클 등등)	
}
