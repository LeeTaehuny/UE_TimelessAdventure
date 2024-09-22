// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_WeaponBase.h"

#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"

ATA_WeaponBase::ATA_WeaponBase()
{
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
