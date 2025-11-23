// Fill out your copyright notice in the Description page of Project Settings.

#include "FROGECT.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FrogEdibleActorComponent.h"
#include "FrogSpitProjectile.h"

AFrogSpitProjectile::AFrogSpitProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFrogSpitProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AFrogSpitProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFrogSpitProjectile::GetEdibleActorData(UFrogEdibleActorComponent* EdibleActorComponent)
{
	if (!EdibleActorComponent || !EdibleActorComponent->MeshData || !EdibleActorComponent->ColliderData)
		return;

	if (RootComponent)
	{
		RootComponent->DestroyComponent();
		RootComponent = nullptr;
	}

	// ---------------------------
	// 1. Collider 기반 RootComponent 생성 (스케일 1)
	// ---------------------------
	UPrimitiveComponent* NewRootCollider = nullptr;

	if (USphereComponent* SphereSource = Cast<USphereComponent>(EdibleActorComponent->ColliderData))
	{
		USphereComponent* SphereTarget = NewObject<USphereComponent>(this);
		if (SphereTarget)
		{
			SphereTarget->SetSphereRadius(SphereSource->GetUnscaledSphereRadius());

			// Overlap 설정 먼저 적용
			SphereTarget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SphereTarget->SetCollisionResponseToAllChannels(ECR_Overlap);

			// RootComponent로 설정
			RootComponent = SphereTarget;

			// Attach & Register
			SphereTarget->SetupAttachment(nullptr);
			SphereTarget->RegisterComponent();

			NewRootCollider = SphereTarget;
		}
	}
	else if (UBoxComponent* BoxSource = Cast<UBoxComponent>(EdibleActorComponent->ColliderData))
	{
		UBoxComponent* BoxTarget = NewObject<UBoxComponent>(this);
		if (BoxTarget)
		{
			BoxTarget->SetBoxExtent(BoxSource->GetUnscaledBoxExtent());

			BoxTarget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			BoxTarget->SetCollisionResponseToAllChannels(ECR_Overlap);

			RootComponent = BoxTarget;
			BoxTarget->SetupAttachment(nullptr);
			BoxTarget->RegisterComponent();

			NewRootCollider = BoxTarget;
		}
	}
	else if (UCapsuleComponent* CapsuleSource = Cast<UCapsuleComponent>(EdibleActorComponent->ColliderData))
	{
		UCapsuleComponent* CapsuleTarget = NewObject<UCapsuleComponent>(this);
		if (CapsuleTarget)
		{
			CapsuleTarget->SetCapsuleHalfHeight(CapsuleSource->GetUnscaledCapsuleHalfHeight());
			CapsuleTarget->SetCapsuleRadius(CapsuleSource->GetUnscaledCapsuleRadius());

			CapsuleTarget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CapsuleTarget->SetCollisionResponseToAllChannels(ECR_Overlap);

			RootComponent = CapsuleTarget;
			CapsuleTarget->SetupAttachment(nullptr);
			CapsuleTarget->RegisterComponent();

			NewRootCollider = CapsuleTarget;
		}
	}
	else if (UStaticMeshComponent* MeshSource = Cast<UStaticMeshComponent>(EdibleActorComponent->ColliderData))
	{
		UStaticMeshComponent* MeshTarget = NewObject<UStaticMeshComponent>(this);
		if (MeshTarget)
		{
			MeshTarget->SetStaticMesh(MeshSource->GetStaticMesh());
			MeshTarget->SetWorldScale3D(FVector(1.f));

			MeshTarget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeshTarget->SetCollisionResponseToAllChannels(ECR_Overlap);

			RootComponent = MeshTarget;
			MeshTarget->SetupAttachment(nullptr);
			MeshTarget->RegisterComponent();

			NewRootCollider = MeshTarget;
		}
	}

	// ---------------------------
	// 2. Mesh Component 생성 및 Root에 attach
	// ---------------------------
	if (UStaticMeshComponent* MeshSource = EdibleActorComponent->MeshData)
	{
		UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(this);
		if (NewMesh)
		{
			NewMesh->SetStaticMesh(MeshSource->GetStaticMesh());

			if (EdibleActorComponent->MaterialData)
				NewMesh->SetMaterial(0, EdibleActorComponent->MaterialData);

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
	SetActorScale3D(EdibleActorComponent->ScaleData);
}
