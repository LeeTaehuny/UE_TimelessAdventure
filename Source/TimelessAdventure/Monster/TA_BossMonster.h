// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster/TA_AIState.h"
#include "Interface/MonsterInterface.h"
#include "TA_BossMonster.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_BossMonster : public ACharacter, public IMonsterInterface
{
	GENERATED_BODY()

public:
	ATA_BossMonster();

protected:
	virtual void BeginPlay() override;

public:
	FOnAttackEndDelegate OnAttackEndDelegate;

	virtual void SetAIAttackDelegate(const FOnAttackEndDelegate& InOnAttackFinished) override;
	virtual void ChangeState(EBossState NewState) override;
	virtual void RangedAttack() override;

protected:
	// 돌 던지기 공격
	void ThrowStone();
	void ThrowStoneEnd(class UAnimMontage* Montage, bool IsEnded);
	// 점프 근접 공격
	void JumpAttack();
	void JumpAttackEnd(class UAnimMontage* Montage, bool IsEnded);

	// 돌 스폰
	virtual void SpawnStone() override;
	// 돌 던지기
	virtual void Throw() override;

	// 데미지 반환
	FORCEINLINE virtual float GetDamage() override { return Damage; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class UAnimMontage> JumpAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class UAnimMontage> ThrowAttackMontage;

private:
	UPROPERTY(EditAnywhere, Category = "Monster")
	TSubclassOf<class ATA_ThrowStone> StoneClass;

	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class ATA_ThrowStone> TempStone;

	UPROPERTY(EditAnywhere, Category ="Monster")
	FName StoneSocketName;

private:
	UPROPERTY(EditAnywhere, Category = "Monster")
	float MaxHp;

	UPROPERTY(EditAnywhere, Category = "Monster")
	float CurrentHp;

	UPROPERTY(EditAnywhere, Category = "Monster")
	float Damage;

private:
	UPROPERTY(VisibleAnywhere, Category = "Monster")
	EBossState BossState;
};
