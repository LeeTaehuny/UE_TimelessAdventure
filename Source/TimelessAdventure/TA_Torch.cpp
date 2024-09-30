// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_Torch.h"

// Sets default values
ATA_Torch::ATA_Torch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Initialize the Torch Mesh and set it as the Root component
	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorchMesh"));
	RootComponent = TorchMesh;

	// Disable collision for the torch
	TorchMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Disable physics simulation for the torch
	TorchMesh->SetSimulatePhysics(false);

	// Ensure that the torch doesn't affect the camera or movement
	TorchMesh->SetEnableGravity(false);  // Gravity off in case it's floating
}


// Called when the game starts or when spawned
void ATA_Torch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATA_Torch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

