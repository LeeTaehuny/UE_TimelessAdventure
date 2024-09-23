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

	// 화살 줄의 로컬 위치를 구해줍니다.
	// * GetTransform().InverseTransformPosition(소켓 위치) : 월드 상의 소켓 위치를 월드변환행렬의 역함수를 곱해 로컬 위치를 구합니다.
	BaseLocation = GetTransform().InverseTransformPosition(WeaponMesh->GetSocketLocation(StringSocketName));
}

void ATA_Bow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsHold)
	{
		// 화살 위치를 전달받아 StringLocation에 저장합니다.
		StringLocation = GetArrowSocketLocation(TempMesh);
		// 화살 줄 Bone의 위치를 당겨진 상태로 설정합니다.
		WeaponMesh->SetBoneLocationByName(StringSocketName, StringLocation, EBoneSpaces::WorldSpace);
	}
	else
	{
		// 화살 줄 Bone의 위치를 기본 위치로 설정합니다.
		WeaponMesh->SetBoneLocationByName(StringSocketName, BaseLocation, EBoneSpaces::ComponentSpace);
	}
}

FVector ATA_Bow::GetArrowSocketLocation(USkeletalMeshComponent* Mesh)
{
	// 화살 소켓 위치를 반환합니다.
	return Mesh->GetSocketLocation(ArrowSocketName);
}

void ATA_Bow::EquipWeapon(USkeletalMeshComponent* Mesh)
{
	Super::EquipWeapon(Mesh);

	// 화살통 스폰
	Quiver = GetWorld()->SpawnActor<AActor>(QuiverClass, GetActorTransform());
	Quiver->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, QuiverSocketName);
}
