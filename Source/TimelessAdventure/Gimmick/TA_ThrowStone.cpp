// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TA_ThrowStone.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ATA_ThrowStone::ATA_ThrowStone()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereComponent);

	InitSpeed = 300.0f;
	MaxSpeed = 300.0f;
	HomingAccelerationMagnitude = 300.0f;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjectileMovementComponent->InitialSpeed = InitSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void ATA_ThrowStone::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovementComponent->SetActive(false);
}

void ATA_ThrowStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATA_ThrowStone::Fire(AActor* Target, FVector Direction)
{
	// 타겟 설정
	ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
	
	// 방향 및 속도 지정
	ProjectileMovementComponent->Velocity = Direction * InitSpeed;

	// 발사
	ProjectileMovementComponent->SetActive(true);
}

