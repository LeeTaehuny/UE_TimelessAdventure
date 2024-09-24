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

public:
	// 화살 생성 함수
	void SpawnArrow(USkeletalMeshComponent* Mesh);
	// 화살 발사 함수
	void ShootArrow();
	// 화살 삭제 함수
	void RemoveArrow();
	// 무기 삭제 함수
	virtual void RemoveWeapon() override;

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

	UPROPERTY(EditAnywhere, Category = "Bow", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> QuiverClass;

	UPROPERTY(VisibleAnywhere, Category = "Bow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> Quiver;

	UPROPERTY(EditAnywhere, Category = "Bow", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATA_Arrow> ArrowClass;

	UPROPERTY(EditAnywhere, Category = "Bow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ATA_Arrow> Arrow;

	bool bIsHold;

	FVector BaseLocation;
	FVector StringLocation;
};
