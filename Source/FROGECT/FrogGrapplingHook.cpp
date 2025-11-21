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

}

void AFrogGrapplingHook::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	AFrogPlayerCharacter* Player = Cast<AFrogPlayerCharacter>(GetOwner());

	if (!Player || !Other) return;
	{
		if (Other->FindComponentByClass<UFrogEdibleActorComponent>())
		{
			// 낼룸
			Other->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Player->OnToungeAttached(*Other);
		}
		else
		{
			// 그래플링
			Player->OnHookAttached(Hit.ImpactPoint);
		}

		UE_LOG(LogTemp, Warning, TEXT("Hook Attached at Location: %s"), *Hit.ImpactPoint.ToString());
	}
}