// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster/TA_AIState.h"
#include "Interface/MonsterInterface.h"
#include "TA_BossMonster.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedBossHpDelegate, float Percent);

UCLASS()
class TIMELESSADVENTURE_API ATA_BossMonster : public ACharacter, public IMonsterInterface
{
	GENERATED_BODY()

public:
	ATA_BossMonster();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	FOnAttackEndDelegate OnAttackEndDelegate;
	FOnJumpBackEndDelegate OnJumpEndDelegate;
	FOnChangedBossHpDelegate OnChangedBossHpDelegate;

	virtual void SetAIAttackDelegate(const FOnAttackEndDelegate& InOnAttackFinished) override;
	virtual void SetAIJumpDelegate(const FOnJumpBackEndDelegate& OnJumpEnd) override;
	virtual void ChangeState(EBossState NewState) override;
	virtual void RangedAttack() override;
	virtual void MeleeAttack() override;
	virtual void JumpBack(float Distance) override;
	virtual float GetHealthPercent() override;

protected:
	// * 원거리 (거리 1500)
	// 돌 던지기 공격
	void ThrowStone();
	void ThrowStoneEnd(class UAnimMontage* Montage, bool IsEnded);
	// 점프 근접 공격
	void JumpAttack();
	void JumpAttackEnd(class UAnimMontage* Montage, bool IsEnded);
	// 근접 텔레포트 공격
	void TeleportAttack();
	void TeleportCallBack();
	void TeleportAttackEnd(class UAnimMontage* Montage, bool IsEnded);


	// * 근거리
	// 근접 일반 공격
	void BaseAttack();
	void BaseAttackEnd(class UAnimMontage* Montage, bool IsEnded);
	// 근접 넉백 공격
	void KnockbackAttack();
	void KnockbackAttackEnd(class UAnimMontage* Montage, bool IsEnded);

	void JumpBackEnd(class UAnimMontage* Montage, bool IsEnded);

	// 돌 스폰
	virtual void SpawnStone() override;
	// 돌 던지기
	virtual void Throw() override;

	// 데미지 반환
	FORCEINLINE virtual float GetDamage() override { return Damage; }
	// 공격 체크 - 일반
	virtual void BaseAttackCheck() override;
	// 공격 체크 - 넉백
	virtual void KnockbackAttackCheck() override;
	// 공격 체크 - 점프
	virtual void JumpAttackCheck() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> FXComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> FX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> HitFX;

private:
	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class UAnimMontage> JumpAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class UAnimMontage> ThrowAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class UAnimMontage> BaseAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class UAnimMontage> KnockbackAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class UAnimMontage> JumpBackMontage;

	UPROPERTY(EditAnywhere, Category = "Monster")
	TObjectPtr<class UAnimMontage> TeleportAttackMontage;

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

	UPROPERTY(EditAnywhere, Category = "Monster")
	float AttackDistance;

// Action
private:
	bool bCanThrowStoneAttack;
	bool bCanJumpBack;

	UPROPERTY(EditAnywhere, Category = "Monster")
	float ThrowStoneCoolTime;

	UPROPERTY(EditAnywhere, Category = "Monster")
	float JumpBackCoolTime;

private:
	UPROPERTY(VisibleAnywhere, Category = "Monster")
	EBossState BossState;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void Hit(float HitDamage);
	virtual void Die() override;

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UTA_BossHp> BossHpWidgetClass;

	UPROPERTY()
	TObjectPtr<class UTA_BossHp> BossHpWidget;

private:
	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> TeleportSound;
};
