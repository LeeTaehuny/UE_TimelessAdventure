// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HR_ItemBase.h"
#include "HR_WeaponBase.generated.h"

// 무기 Enum
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_Sword,		// 검
	WT_Bow,			// 활
	WT_Torch		// 횃불
};

UCLASS()
class TIMELESSADVENTURE_API AHR_WeaponBase : public AHR_ItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_WeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// member
protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	EWeaponType WeaponType;
	

// Weapon Equip
public:
	void Equip(FName socketName, USkeletalMeshComponent* skeletalMeshComp);
	
};
