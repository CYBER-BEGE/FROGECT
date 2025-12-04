// Fill out your copyright notice in the Description page of Project Settings.

#include "FROGECT.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FrogEdibleActorComponent.h"
#include "FrogSpitProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

AFrogSpitProjectile::AFrogSpitProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AFrogSpitProjectile::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("%s: 스폰됨"), *this->GetActorLabel());

	if (Owner) 
	{
		UE_LOG(LogTemp, Display, TEXT("%s: 발사자: %s"), *this->GetActorLabel(), *Owner->GetActorLabel());
	}
	else UE_LOG(LogTemp, Warning, TEXT("%s: No Owner"), *this->GetActorLabel());

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AFrogSpitProjectile::OnSpitProjectileHit);
	
	
	
	
	// RootComponent가 PrimitiveComponent면 Hit 이벤트 바인딩
	//if (UPrimitiveComponent* PrimRoot = Cast<UPrimitiveComponent>(RootComponent))
	//{
		//PrimRoot->OnComponentHit.AddDynamic(this, &AFrogSpitProjectile::OnProjectileHit);
		//if (Owner) PrimRoot->IgnoreActorWhenMoving(Owner, true);

		/*


		// 날아갈 때 Physics는 끄고 Gravity도 끔
		PrimRoot->SetSimulatePhysics(true);
		PrimRoot->SetEnableGravity(false);
		
	}

	StartDestroyTimer(); // 스폰 시 5초 자동 파괴 타이머
}

// Called every frame
void AFrogSpitProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFrogSpitProjectile::StartDestroyTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("%s: DestroyTimer 호출됨"), *this->GetActorLabel());

	// 발사 후 5초 뒤 투사체 파괴(투사체 수명)
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AFrogSpitProjectile::DestroyProjectile, 2.0f, false);*/
}

void AFrogSpitProjectile::OnSpitProjectileHit(const FHitResult& ImpactResult)
{
	AActor* HitActor = ImpactResult.GetActor();
	UE_LOG(LogTemp, Display, TEXT("%s: %s와 충돌함"), *this->GetActorLabel(), *HitActor->GetActorLabel());
}

/*
void AFrogSpitProjectile::OnProjectileHit(

	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	//if (bHasHit || !HitComp) return; // 이미 Hit 처리했으면 무시
	UE_LOG(LogTemp, Display, TEXT("%s: %s와 충돌함"), *this->GetActorLabel(), *OtherActor->GetActorLabel());
	
	if (Owner) 
	{
		UE_LOG(LogTemp, Display, TEXT("Owner: %s"), *Owner->GetActorLabel());
	}
	else UE_LOG(LogTemp, Display, TEXT("No Owner"));

	UE_LOG(LogTemp, Display, TEXT("뱉은거랑 %s랑 충돌 있음"), *OtherActor->GetActorLabel());

	bHasHit = true; // 최초 Hit 처리

	// 기존 타이머 초기화
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);

	// ProjectileMovement 중지
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Deactivate();
	}

	// Physics 활성화 + Gravity 적용
	//HitComp->SetSimulatePhysics(false);
	HitComp->SetEnableGravity(true);

	// 타이머 재시작 (충돌 후 5초 뒤 파괴)
	StartDestroyTimer();
}*/

void AFrogSpitProjectile::CloneEdibleActor(UFrogEdibleActorComponent* EdibleActorComponent)
{
	if (!EdibleActorComponent) return;

	/* 기존 RootComponent 제거 */
	if (RootComponent)
	{
		RootComponent->DestroyComponent();
		RootComponent = nullptr;
	}

	const FEdibleActorData& Data = EdibleActorComponent->GetEdibleData(); // 데이터 가져오기

	// ---------------------------
	// 1. Collider 기반 RootComponent 생성 (스케일 1)
	// ---------------------------
	UPrimitiveComponent* NewRootCollider = nullptr;

	if (USphereComponent* SphereSource = Cast<USphereComponent>(Data.ColliderData))
	{
		USphereComponent* SphereTarget = NewObject<USphereComponent>(this);
		if (SphereTarget)
		{
			SphereTarget->SetSphereRadius(SphereSource->GetUnscaledSphereRadius());

			// Overlap 설정 먼저 적용
			SphereTarget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SphereTarget->SetCollisionResponseToAllChannels(ECR_Block);

			// RootComponent로 설정
			RootComponent = SphereTarget;

			// Attach & Register
			SphereTarget->SetupAttachment(nullptr);
			SphereTarget->RegisterComponent();

			NewRootCollider = SphereTarget;
		}
	}
	else if (UBoxComponent* BoxSource = Cast<UBoxComponent>(Data.ColliderData))
	{
		UBoxComponent* BoxTarget = NewObject<UBoxComponent>(this);
		if (BoxTarget)
		{
			BoxTarget->SetBoxExtent(BoxSource->GetUnscaledBoxExtent());

			BoxTarget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			BoxTarget->SetCollisionResponseToAllChannels(ECR_Block);

			RootComponent = BoxTarget;
			BoxTarget->SetupAttachment(nullptr);
			BoxTarget->RegisterComponent();

			NewRootCollider = BoxTarget;
		}
	}
	else if (UCapsuleComponent* CapsuleSource = Cast<UCapsuleComponent>(Data.ColliderData))
	{
		UCapsuleComponent* CapsuleTarget = NewObject<UCapsuleComponent>(this);
		if (CapsuleTarget)
		{
			CapsuleTarget->SetCapsuleHalfHeight(CapsuleSource->GetUnscaledCapsuleHalfHeight());
			CapsuleTarget->SetCapsuleRadius(CapsuleSource->GetUnscaledCapsuleRadius());

			CapsuleTarget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CapsuleTarget->SetCollisionResponseToAllChannels(ECR_Block);

			RootComponent = CapsuleTarget;
			CapsuleTarget->SetupAttachment(nullptr);
			CapsuleTarget->RegisterComponent();

			NewRootCollider = CapsuleTarget;
		}
	}
	else if (UStaticMeshComponent* MeshSource = Cast<UStaticMeshComponent>(Data.ColliderData))
	{
		UStaticMeshComponent* MeshTarget = NewObject<UStaticMeshComponent>(this);
		if (MeshTarget)
		{
			MeshTarget->SetStaticMesh(MeshSource->GetStaticMesh());
			MeshTarget->SetWorldScale3D(FVector(1.f));

			MeshTarget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeshTarget->SetCollisionResponseToAllChannels(ECR_Block);

			RootComponent = MeshTarget;
			MeshTarget->SetupAttachment(nullptr);
			MeshTarget->RegisterComponent();

			NewRootCollider = MeshTarget;
		}
	}

	// ---------------------------
	// 2. Mesh Component 생성 및 Root에 attach
	// ---------------------------
	if (UStaticMeshComponent* MeshSource = Data.MeshData)
	{
		UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(this);
		if (NewMesh)
		{
			NewMesh->SetStaticMesh(MeshSource->GetStaticMesh());

			if (Data.MaterialData)
				NewMesh->SetMaterial(0, Data.MaterialData);

			NewMesh->SetWorldScale3D(FVector(1.f));

			// 메시 Collider도 Overlap 설정
			NewMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			NewMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

			NewMesh->SetupAttachment(RootComponent);
			NewMesh->RegisterComponent();
		}
	}

	// ---------------------------
	// 3. Actor 스케일 적용 (전체 크기)
	// ---------------------------
	SetActorScale3D(Data.ScaleData);

	ProjectileMovement->SetUpdatedComponent(RootComponent);

}
