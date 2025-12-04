// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FrogEdibleActorComponent.generated.h"

// Edible Actor 외형 값 저장하는 구조체
USTRUCT(BlueprintType)
struct FEdibleActorData
{
	GENERATED_BODY()

public:
	UStaticMeshComponent* MeshData = nullptr;
	UMaterialInterface* MaterialData = nullptr;
	UPrimitiveComponent* ColliderData = nullptr;
	FVector ScaleData = FVector(1.f);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGECT_API UFrogEdibleActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFrogEdibleActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FEdibleActorData CachedData;

	//Owner Actor의 정보 저장
	void CacheEdibleActorData();

public:
	const FEdibleActorData& GetEdibleData() const { return CachedData; }
};
