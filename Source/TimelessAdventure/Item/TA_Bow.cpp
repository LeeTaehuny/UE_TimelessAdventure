// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_Bow.h"

#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

ATA_Bow::ATA_Bow()
{
	WeaponType = EWeaponType::WT_Bow;

	WeaponSocketName = TEXT("BowSocket");
}

void ATA_Bow::BeginPlay()
{
	Super::BeginPlay();

	// ȭ�� ���� ���� ��ġ�� �����ݴϴ�.
	// * GetTransform().InverseTransformPosition(���� ��ġ) : ���� ���� ���� ��ġ�� ���庯ȯ����� ���Լ��� ���� ���� ��ġ�� ���մϴ�.
	BaseLocation = GetTransform().InverseTransformPosition(WeaponMesh->GetSocketLocation(StringSocketName));
}

void ATA_Bow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsHold)
	{
		// ȭ�� ��ġ�� ���޹޾� StringLocation�� �����մϴ�.
		StringLocation = GetArrowSocketLocation(TempMesh);
		// ȭ�� �� Bone�� ��ġ�� ����� ���·� �����մϴ�.
		WeaponMesh->SetBoneLocationByName(StringSocketName, StringLocation, EBoneSpaces::WorldSpace);
	}
	else
	{
		// ȭ�� �� Bone�� ��ġ�� �⺻ ��ġ�� �����մϴ�.
		WeaponMesh->SetBoneLocationByName(StringSocketName, BaseLocation, EBoneSpaces::ComponentSpace);
	}
}

FVector ATA_Bow::GetArrowSocketLocation(USkeletalMeshComponent* Mesh)
{
	// ȭ�� ���� ��ġ�� ��ȯ�մϴ�.
	return Mesh->GetSocketLocation(ArrowSocketName);
}

void ATA_Bow::EquipWeapon(USkeletalMeshComponent* Mesh)
{
	Super::EquipWeapon(Mesh);

	// ȭ���� ����
	Quiver = GetWorld()->SpawnActor<AActor>(QuiverClass, GetActorTransform());
	Quiver->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, QuiverSocketName);
}
