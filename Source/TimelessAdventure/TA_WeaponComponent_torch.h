// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_WeaponComponent.h"
#include "TA_WeaponComponent_torch.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TIMELESSADVENTURE_API UTA_WeaponComponent_torch : public UTA_WeaponComponent
{
	GENERATED_BODY()
public:
	UTA_WeaponComponent_torch();
	virtual void Attack() override;

	int32 CurrentComboIndex;
	int32 QueuedAttacks;

	void StartAttackSequence(UAnimInstance* AnimInstance, ACharacter* OwnerCharacter);
	bool bIsComboActive;
	void ResetCombo();
	void UpdateCombo();
	virtual void CancelAttackAndMove() override;
	void OnNotifyReceived(UAnimNotify* Notify);
};

