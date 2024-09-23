// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/TA_WeaponBase.h"
#include "TA_Bow.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API ATA_Bow : public ATA_WeaponBase
{
	GENERATED_BODY()
	
public:
	ATA_Bow();

public:
	FORCEINLINE void SetIsHold(bool InValue) { bIsHold = InValue; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	// 화살 위치 반환용 함수
	FVector GetArrowSocketLocation(USkeletalMeshComponent* Mesh);

	virtual void EquipWeapon(class USkeletalMeshComponent* Mesh) override;

private:
	UPROPERTY(EditAnywhere, Category = "SocketName", meta = (AllowPrivateAccess = "true"))
	FName StringSocketName;

	UPROPERTY(EditAnywhere, Category = "SocketName", meta = (AllowPrivateAccess = "true"))
	FName ArrowSocketName;

	UPROPERTY(EditAnywhere, Category = "SocketName", meta = (AllowPrivateAccess = "true"))
	FName QuiverSocketName;

	UPROPERTY(EditAnywhere, Category = "SocketName", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> QuiverClass;

	UPROPERTY(VisibleAnywhere, Category = "SocketName", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> Quiver;

	bool bIsHold;

	FVector BaseLocation;
	FVector StringLocation;
};
