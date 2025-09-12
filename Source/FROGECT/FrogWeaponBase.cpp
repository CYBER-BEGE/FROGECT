// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogWeaponBase.h"

// Sets default values
AFrogWeaponBase::AFrogWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// 데미지 컴포넌트 어태치
	FrogDamageComponent = CreateDefaultSubobject<UFrogDamageComponent>(TEXT("Damage Component"));
	FrogDamageComponent->SetupAttachment(Mesh);

	// 콜리전 기본 OFF
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

