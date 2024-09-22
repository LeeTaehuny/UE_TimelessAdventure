// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_WeaponComponent.h"
#include "TA_WeaponComponent_sword.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TIMELESSADVENTURE_API UTA_WeaponComponent_sword : public UTA_WeaponComponent
{
	GENERATED_BODY()

public:
	UTA_WeaponComponent_sword();

	void CancelAttackAndMove();
	void OnNotifyReceived(UAnimNotify* Notify);
	virtual void InitializeComponent() override;
	virtual void Attack() override;
private:
	int32 CurrentComboIndex;
	int32 QueuedAttacks;
	bool bIsComboActive;

	void StartAttackSequence(UAnimInstance* AnimInstance, ACharacter* OwnerCharacter);
	void ResetCombo();
	void UpdateCombo();
};
