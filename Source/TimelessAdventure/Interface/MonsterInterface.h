// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Monster/TA_AIState.h"
#include "MonsterInterface.generated.h"

DECLARE_DELEGATE(FOnAttackEndDelegate)
DECLARE_DELEGATE(FOnJumpBackEndDelegate)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TIMELESSADVENTURE_API IMonsterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ChangeState(EBossState NewState) = 0;
	virtual void RangedAttack() = 0;
	virtual void MeleeAttack() = 0;

	virtual void SpawnStone() = 0;
	virtual void Throw() = 0;
	virtual float GetDamage() = 0;

	virtual	void BaseAttackCheck() = 0;
	virtual	void KnockbackAttackCheck() = 0;

	virtual void JumpBack(float Distance) = 0;

	virtual void SetAIAttackDelegate(const FOnAttackEndDelegate& OnAttackEnd) = 0;
	virtual void SetAIJumpDelegate(const FOnJumpBackEndDelegate& OnJumpEnd) = 0;

	virtual float GetHealthPercent() = 0;
	virtual void Die() = 0;
};
