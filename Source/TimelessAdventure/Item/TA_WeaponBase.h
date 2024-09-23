// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/TA_ItemBase.h"
#include "TA_WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_Sword,	// 검
	WT_Bow,		// 활
	WT_Torch,	// 횃불
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
	// 부착할 소켓 이름
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FName WeaponSocketName;

	// 무기 메쉬
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UPoseableMeshComponent> WeaponMesh;

	// 무기 타입
	EWeaponType WeaponType;

	// 임시 스켈레톤 저장
	TObjectPtr<class USkeletalMeshComponent> TempMesh;
};
