// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Sangeon/TA_Movable.h"

// Sets default values
ATA_Movable::ATA_Movable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovableMesh"));
	MovableMesh->SetupAttachment(RootComponent);

	MovableMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ATA_Movable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATA_Movable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* ATA_Movable::GetMeshComponent()
{
	return MovableMesh;
}

void ATA_Movable::SelfDrop()
{
	if(MovableMesh == nullptr) return;
	if(MovableMesh)
	{
		MovableMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		MovableMesh->SetSimulatePhysics(true);
		MovableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		IsHeld = false;
	}
	else
	{
		return;
	}
}

bool ATA_Movable::SelfPickup(USceneComponent* ParentComp)
{
	if(IsLocked)
	{
		return false;
	}
	else
	{
		if(ParentComp)
		{
			FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld,true);
			MovableMesh->AttachToComponent(ParentComp, AttachRules);

			MovableMesh->SetSimulatePhysics(false);

			MovableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			IsHeld = true;
			return true;
		}
		return false;
	}
}

