// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Sangeon/TA_Movable.h"

// Sets default values
ATA_Movable::ATA_Movable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovableMesh"));
	RootComponent = MovableMesh;

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

void ATA_Movable::SelfDrop()
{
	if(MovableMesh)
	{
		MovableMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		MovableMesh->SetSimulatePhysics(true);
		MovableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		IsHeld = false;
	}
}

bool ATA_Movable::SelfPickup(UPrimitiveComponent* ParentComp)
{
	if(Locked)
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

