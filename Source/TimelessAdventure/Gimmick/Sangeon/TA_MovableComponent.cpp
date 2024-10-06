// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Sangeon/TA_MovableComponent.h"

// Sets default values for this component's properties
UTA_MovableComponent::UTA_MovableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTA_MovableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTA_MovableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


UStaticMeshComponent* UTA_MovableComponent::GetMeshComponent()
{
	return MovableMesh;
}

void UTA_MovableComponent::SelfDrop()
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
/*
bool UTA_MovableComponent::SelfPickup(USceneComponent* ParentComp)
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

*/

bool UTA_MovableComponent::SelfPickup(USceneComponent* ParentComp)
{
	if(IsLocked)
	{
		return false;
	}
	else
	{
		if(ParentComp && MovableMesh)
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


