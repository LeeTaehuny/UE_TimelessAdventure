// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/TA_ItemBase.h"
#include "TA_WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_Sword,	// ��
	WT_Bow,		// Ȱ
	WT_Torch,	// ȶ��
};

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API ATA_WeaponBase : public ATA_ItemBase
{
	GENERATED_BODY()
	
public:
	ATA_WeaponBase();

public:
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; }

public:
	virtual void EquipWeapon(class USkeletalMeshComponent* Mesh);

protected:
	// ������ ���� �̸�
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FName WeaponSocketName;

	// ���� �޽�
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UPoseableMeshComponent> WeaponMesh;

	// ���� Ÿ��
	EWeaponType WeaponType;

	// �ӽ� ���̷��� ����
	TObjectPtr<class USkeletalMeshComponent> TempMesh;
};
