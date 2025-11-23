// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFrogProjectileBase::AFrogProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 구체 콜리전 컴포넌트를 생성하고 루트로 지정
	RootComponent = CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));

	CollisionComponent->SetSphereRadius(16.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);// 충돌 쿼리 및 물리 활성화
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);			// 모든 채널에 대해 충돌 Block 설정
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;		// 캐릭터가 이 컴포넌트 위로 올라설 수 없음

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->InitialSpeed = 5000.0f; // 초기 속도 설정
	ProjectileMovement->MaxSpeed = 5000.0f;		// 최대 속도 설정
	ProjectileMovement->bShouldBounce = false;	// 바운스 활성화

}

// Called when the game starts or when spawned
void AFrogProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFrogProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFrogProjectileBase::DestroyProjectile()
{
	Destroy();
}

void AFrogProjectileBase::BounceProjectile()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = -ProjectileMovement->Velocity;
	}
}