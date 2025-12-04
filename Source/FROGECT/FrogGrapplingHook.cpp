// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogGrapplingHook.h"
#include "FrogPlayerCharacter.h"
#include "FrogEdibleActorComponent.h"

// Sets default values
AFrogGrapplingHook::AFrogGrapplingHook()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HookCable = CreateDefaultSubobject<UCableComponent>(TEXT("HookCable"));
	HookCable->SetupAttachment(RootComponent);

	HookCable->EndLocation = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AFrogGrapplingHook::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFrogGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ToungeReturn(DeltaTime);
}

void AFrogGrapplingHook::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	AFrogPlayerCharacter* Player = Cast<AFrogPlayerCharacter>(GetOwner());

	if (!Player || !Other) return;
	{
		if (Other->FindComponentByClass<UFrogEdibleActorComponent>()) // 이럴 필요가 없잖아 애초에 다른걸 호출하던가
		{
			// 낼룸
			Other->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			Player->OnToungeAttached(*Other);
			Player->EatingActor = Other;
		}
		else
		{
			// 매달리기
			Player->OnHookAttached(Hit.ImpactPoint);
		}

		UE_LOG(LogTemp, Warning, TEXT("Tounge Attached at Location: %s"), *Hit.ImpactPoint.ToString());
	}
}

void AFrogGrapplingHook::ReturnProjectile()
{
	if (!GetOwner()) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Tounge returning to %s"), *GetOwner()->GetName());

	bToungeReturning = true;

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Tounge 속도 초기화
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = FVector::ZeroVector;
	}
}

void AFrogGrapplingHook::ToungeReturn(float DeltaTime)
{
	if (!bToungeReturning || !GetOwner()) return;

	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector Dir = (OwnerLocation - GetActorLocation()).GetSafeNormal();

	// 매 프레임 위치 갱신
	FVector NewLocation = GetActorLocation() + Dir * ReturnSpeed * DeltaTime;
	SetActorLocation(NewLocation, true);

	if (FVector::DistSquared(NewLocation, OwnerLocation) < FMath::Square(100.f))
	{
		OnProjectileReturned.Broadcast();

		Destroy();
		bToungeReturning = false;
	}
}