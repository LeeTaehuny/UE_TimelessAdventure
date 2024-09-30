// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_InputComponent.h"
#include "TA_PlayerComponentBase.h"
#include "TA_WeaponComponent.h"
#include "TA_CombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnZeroHealthDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMELESSADVENTURE_API UTA_CombatComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()

public:
	UTA_CombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Getter & Setter
public:
	// Return Stamian
	float GetStaminaPercent();
	// Stamina Increase(false)/Decrease(true)
	void SetUseStamina(bool Value);
	// Stamina instant increase/decrease function (true : Decrese, fasle : Increase | Percent : Stamina Percent)
	void UpdateStamina(bool Value, float Percent);
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void ChangeToIdleState();

	// Delegate
public:
	// Stamina Zero
	FOnZeroHealthDelegate ZeroHealthDelegate;
	
	// 장착된 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UTA_WeaponComponent* CurrentWeapon;  // 정확한 타입으로 선언

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ATA_Sword> SwordClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ATA_Torch> TorchClass;

	
	// 무기 교체 함수
	void EquipWeapon(UTA_WeaponComponent* NewWeapon);
	UPROPERTY()
	AActor* SpawnedWeaponActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EquipSwordMontage;

	
	// 공격 함수
	void Attack();
	void OnNotifyReceived(UAnimNotify* Notify);

private:
	// Initialize (BeginPlay)
	void Init();

	FVector2D PreviousMovementVector;
private:
	// Stamina
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float CurrentStamina;

	// Hp
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHp;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float CurrrentHp;

	// Judgment Stamina increase/decrease (true : decrease, false : increase)
	bool bUseStamina;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	// Stamina increase/decrease amount 
	float UseStaminaPercent;
};
