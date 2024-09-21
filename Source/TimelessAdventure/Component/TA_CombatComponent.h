// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_PlayerComponentBase.h"
#include "TA_CombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnZeroHealthDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMELESSADVENTURE_API UTA_CombatComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()

public:
	UTA_CombatComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Getter & Setter
public:
	// 체력 퍼센트 반환 함수
	float GetHealthPercent();
	// 체력 지속 증가/감소 설정 함수 (true : 감소, false : 증가)
	void SetUseHealth(bool Value);
	// 체력 즉시 증가/감소 설정 함수 (true : 감소, fasle : 증가 | Percent : 처리할 체력 퍼센트)
	void UpdateHealth(bool Value, float Percent);
	
	// 현재 공격 여부 반환 함수
	FORCEINLINE bool GetIsAttacking() { return bIsAttacking; }

// Delegate
public:
	// 체력이 0이 되면 사용할 델리게이트
	FOnZeroHealthDelegate ZeroHealthDelegate;

private:
	// 게임 시작시 호출되는 초기화 함수 (BeginPlay)
	void Init();

// Actions
public:
	// 공격
	void Attack();
	// 공격 이동
	void AttackMove();

// ComboAttack
private:
	// 점프 공격
	void JumpAttack();
	// 콤보 공격 시작
	void ComboStart();
	// 콤보 공격 종료
	void EndCombo(class UAnimMontage* Montage, bool IsEnded);

	// 콤보 타이머 시작
	void SetComboTimer();
	// 콤보 체크
	void CheckCombo();

	// 콤보 데이터
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<class UTA_ComboAttackData> ComboAttackData;

	// 콤보 타이머 핸들
	FTimerHandle ComboTimerHandle;

	// 공격 중 판별 변수
	bool bIsAttacking;

	// 콤보 입력 판별 변수
	bool bIsComboInput;

	// 콤보 공격 카운트
	int32 ComboCount;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackMoveForce;

// Animations
private:
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (PrivateAllowAccess = "true"))
	TObjectPtr<class UAnimMontage> JumpAttackMontage;
	
	void JumpAttackMontageEnded(class UAnimMontage* Montage, bool IsEnded);

// Stat
private:
	// 체력
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float CurrentStamina;

	// Hp
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHp;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float CurrrentHp;
	
	// 지속 체력 증가/감소량 
	UPROPERTY(EditAnywhere, Category = "Stat")
	float UseHealthPercent;

	// 지속 체력 증가/감소 판별 (true : 감소, false : 증가)
	bool bUseHealth;
};
