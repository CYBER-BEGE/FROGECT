// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogWeaponBase.h"

// Sets default values
AFrogWeaponBase::AFrogWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = WeaponMesh;

	// 데미지 컴포넌트 어태치
	FrogDamageComponent = CreateDefaultSubobject<UFrogDamageComponent>(TEXT("Damage Component"));

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 콜리전 OFF
	WeaponMesh->SetGenerateOverlapEvents(false); // 오버랩 이벤트 OFF
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
