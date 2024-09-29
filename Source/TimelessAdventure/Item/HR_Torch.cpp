// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_Torch.h"

#include "Particles/ParticleSystemComponent.h"


// Sets default values
AHR_Torch::AHR_Torch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch StaticMesh"));
	RootComponent = WeaponMesh;
	TorchParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Torch Particle"));
	
}

// Called when the game starts or when spawned
void AHR_Torch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_Torch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

