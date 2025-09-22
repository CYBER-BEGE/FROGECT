// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogWeaponBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFrogWeaponBase::AFrogWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/* WeaponColider ------------------------------------------------------------------- */

	// Colider 생성
	WeaponCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = WeaponCollider;
	// Colider 콜리전, 오버랩 비활성화(초기 설정)
	WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollider->SetGenerateOverlapEvents(false);
	// Colider 콜리전 프리셋 설정 
	WeaponCollider->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	
	/* WeaponMesh ---------------------------------------------------------------------- */

	// Mesh 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetupAttachment(WeaponCollider);
	// Mesh 콜리전, 오버랩 비활성화
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetGenerateOverlapEvents(false);
	// Mesh 콜리전 프리셋 설정(전체 무시)
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


	FrogDamageComponent = CreateDefaultSubobject<UFrogDamageComponent>(TEXT("Damage Component")); // 데미지 컴포넌트 어태치
}

// Called when the game starts or when spawned
void AFrogWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &AFrogWeaponBase::OnWeaponOverlap); // Overlap 이벤트 매핑
}

// Called every frame
void AFrogWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFrogWeaponBase::EnableWeaponOverlap()
{
	WeaponCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponCollider->SetGenerateOverlapEvents(true);
}

void AFrogWeaponBase::DisableWeaponOverlap()
{
	WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollider->SetGenerateOverlapEvents(false);
}

void AFrogWeaponBase::OnWeaponOverlap(UPrimitiveComponent* ThisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 /*OtherBodyIndex*/, bool /*bFromSweep*/, const FHitResult& /*SweepResult*/)
{
	UE_LOG(LogTemp, Warning, TEXT("OnWeaponOverlap 진입체크"));

	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;
	UE_LOG(LogTemp, Warning, TEXT("Overlap %s"), *OtherActor->GetActorLabel());

	if (FrogDamageComponent) 
	{
		//FrogDamageComponent->ApplyDamage(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("Overlap %s"), *OtherActor->GetName());
	}
	else 
	{
		//UE_LOG(LogTemp, Warning, TEXT("No Damage Component: %s"), *GetName());
	}
}
