// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_WeaponBase.h"

#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"

ATA_WeaponBase::ATA_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ATA_WeaponBase::EquipWeapon(USkeletalMeshComponent* Mesh)
{
	if (Mesh)
	{
		WeaponMesh->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
		TempMesh = Mesh;
	}
}

void ATA_WeaponBase::RemoveWeapon()
{
	Destroy();
}

void ATA_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
