// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_Arrow.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ATA_Arrow::ATA_Arrow()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	ShaftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShaftMesh"));
	ShaftMesh->SetupAttachment(BaseMesh);

	IronMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IronMesh"));
	IronMesh->SetupAttachment(ShaftMesh);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(IronMesh);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 3000.0f;
	ProjectileMovementComp->MaxSpeed = 5000.0f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
}

void ATA_Arrow::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovementComp->SetActive(false);
}

void ATA_Arrow::FireArrow(FVector Pos)
{
	FVector Direction = (Pos - GetActorLocation()).GetSafeNormal();
	ProjectileMovementComp->Velocity = Direction * ProjectileMovementComp->InitialSpeed;
	ProjectileMovementComp->Activate();
}

void ATA_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

