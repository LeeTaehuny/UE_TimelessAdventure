// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_PlayerComponentBase.h"
#include "TA_CombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnZeroHealthDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeHpDelegate, float Percent);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeStaminaDelegate, float Percent);

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	CS_Idle,			// 기본
	CS_Dash,			// 달리기
	CS_Roll,			// 구르기 (회피)
	CS_Attack,			// 공격
	CS_Special,			// 특수 동작
	CS_Hit,				// 피격
	CS_Die,				// 사망
};

UENUM(BlueprintType)
enum class EEquippedState : uint8
{
	ES_Idle,		// 기본
	ES_Sword,		// 검
	ES_Bow,			// 활
	ES_Torch,		// 횃불
	ES_Cancel,		// 초기화
};

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
	// 체력 퍼센트 반환 함수
	float GetHealthPercent();

	// 현재 들고있는 무기 반환 함수
	FORCEINLINE class ATA_WeaponBase* GetEquippedWeapon() { return EquippedWeapon; }
	// 현재 공격 여부 반환 함수
	FORCEINLINE bool GetIsAttacking() { return bIsAttacking; }
	// 현재 활 Draw 여부 반환 함수
	FORCEINLINE bool GetIsHolding() { return bIsHold; }
	// 현재 방어 여부 반환 함수
	FORCEINLINE bool GetIsGuard() { return bIsGuard; }

	// 줍기 몽타주 반환 함수
	FORCEINLINE class UAnimMontage* GetPickupMontage() { return PickupMontage; }

public:
	void Interaction();
	void HealStat(float HpPercent, float StaminaPercent);
	void SetChangeWeaponState(EEquippedState NewState);

// Delegate
public:
	// 체력이 0이 되면 사용할 델리게이트
	FOnZeroHealthDelegate ZeroHealthDelegate;
	// 체력 변경 델리게이트
	FOnChangeHpDelegate ChangeHpDelegate;
	// 스테미나 변경 델리게이트
	FOnChangeStaminaDelegate ChangeStaminaDelegate;

private:
	// 게임 시작시 호출되는 초기화 함수 (BeginPlay)
	void Init();

public:
	// Walk
	void Walk(FVector ForwardDir, FVector RightDir, FVector2D MovementVector2D);
	// Dash Start
	void DashStart();
	// Dash End
	void DashEnd();
	// Rolling Start
	void RollStart(FVector2D InMovementVector);
	// Rolling End
	void RollEnd(class UAnimMontage* Montage, bool bInterrupted);
	// Jump
	void CombatJump();
	// Attack
	void Attack();
	// Attack Move
	void AttackMove(float InAttackMoveForce);
	// Right click
	void RightClickStart();
	void RightClickEnd();
	// Middle click
	void MiddleClickStart();
	void MiddleClickEnd();

	// Respawn
	void Respawn();

// ComboAttack
private:
	// 점프 공격
	void JumpAttack();
	// 점프 공격 종료
	void JumpAttackEnd(class UAnimMontage* Montage, bool IsEnded);
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
	TMap<EEquippedState, TObjectPtr<class UTA_ComboAttackData>> ComboAttackDatas;

	// 콤보 타이머 핸들
	FTimerHandle ComboTimerHandle;

	// 공격 중 판별 변수
	bool bIsAttacking;

	// 콤보 입력 판별 변수
	bool bIsComboInput;

	// 콤보 공격 카운트
	int32 ComboCount;

// Special Actions
private:
	// 화살 Draw
	void DrawArrow();
	// 화살 Draw End
	void DrawArrowEnd(class UAnimMontage* Montage, bool IsEnded);
	// 화살 Shoot
	void ShootArrow();
	// 화살 Shoot End
	void ReleaseArrowEnd(class UAnimMontage* Montage, bool IsEnded);

	// Guard Start
	void GuardStart();

	// 활 리셋
	void ResetBow();

	bool bIsHold;
	bool bCanShoot;
	bool bIsGuard;

// Animations
private:
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (PrivateAllowAccess = "true"))
	TMap<EEquippedState, TObjectPtr<class UAnimMontage>> JumpAttackMontages;

	// Roll Montage
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> RollMontage;

	// Draw Arrow Montage
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> DrawArrowMontage;

	// Shoot Arrow Montage
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> ShootArrowMontage;

	// Pickup Montage
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> PickupMontage;

	// Hit Montage
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> GuardHitMontage;

	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> DeathMontage;

// Stat
private:
	void UseStamina(float InValue);

	// 체력
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float CurrentStamina;

	// Hp
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHp;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float CurrentHp;
	
	// 지속 체력 증가/감소량 
	UPROPERTY(EditAnywhere, Category = "Stat")
	float UseStaminaPercent;

private:
	// Change Combat State
	void ChangeState(ECombatState NewState);
	// Current Combat State
	ECombatState CombatState;
	// Cache - Prev Combat State
	ECombatState TempState;

	// Current Weapon State
	EEquippedState EquippedState;
	// Temp Weapon State
	EEquippedState TempEquippedState;

private:
	// Walk Speed
	UPROPERTY(EditAnywhere, Category = "Settings")
	float WalkSpeed;

	// Dash Speed
	UPROPERTY(EditAnywhere, Category = "Settings")
	float DashSpeed;

	// 구르기에 사용될 체력 퍼센트
	UPROPERTY(EditAnywhere, Category = "Settings")
	float RollHealthPercent;

	// Zoom 카메라 거리
	UPROPERTY(EditAnywhere, Category = "Zoom", meta = (AllowPrivateAccess = "true"))
	float ZoomInDistance;
	// 기본 카메라 거리
	UPROPERTY(EditAnywhere, Category = "Zoom", meta = (AllowPrivateAccess = "true"))
	float IdleDistance;

// Weapon
private:
	void ChangeWeapon();
	void EquipWeapon();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TMap<EEquippedState, TSubclassOf<class ATA_WeaponBase>> WeaponClassMap;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class ATA_WeaponBase> EquippedWeapon;

// Damage
public:
	void TakeDamage(float DamageAmount, AActor* DamageCauser, FDamageEvent const& DamageEvent);
	
private:
	void Hit(float InDamage, float InPlaySpeed);
	void HitEnd(class UAnimMontage* Montage, bool IsEnded);
	void Die();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> HitFX;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDistance;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDamage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float LaunchDistance;
};
