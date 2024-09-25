// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_PlayerComponentBase.h"
#include "TA_CombatComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipedWeapon : uint8
{
	None,		// none
	Sword,		//
	Bow,		//
	Torch,		// 
};

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
	// 체력 퍼센트 반환 함수
	float GetHealthPercent();
	// 체력 지속 증가/감소 설정 함수 (true : 감소, false : 증가)
	void SetUseHealth(bool Value);
	// 체력 즉시 증가/감소 설정 함수 (true : 감소, fasle : 증가 | Percent : 처리할 체력 퍼센트)
	void UpdateHealth(bool Value, float Percent);

// Delegate
public:
	// 체력이 0이 되면 사용할 델리게이트
	FOnZeroHealthDelegate ZeroHealthDelegate;

private:
	// 게임 시작시 호출되는 초기화 함수 (BeginPlay)
	void Init();

private:
	// 체력
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float CurrentHealth;

	// Hp
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHp;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float CurrrentHp;

	// 지속 체력 증가/감소 판별 (true : 감소, false : 증가)
	bool bUseHealth;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	// 지속 체력 증가/감소량 
	float UseHealthPercent;

protected:
	EEquipedWeapon EquipedWeapon = EEquipedWeapon::Bow;

// Weapon
public:
	// 나중에 Map이나 Array로 저장 고려 
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AHR_Bow> Weapon_Bow;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName BowSocketName = FName("bow_socket_l");
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<class AHR_Bow> BowObject;

// Bow 기능 
public:
	void AimingBowStart();
	void AimingBowEnd();

// Weapon eauip
public:
	void EquipWeapon();
	
};
