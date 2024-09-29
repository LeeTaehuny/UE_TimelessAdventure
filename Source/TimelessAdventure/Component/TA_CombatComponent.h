// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_PlayerComponentBase.h"
#include "TA_CombatComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipedWeapon : uint8
{
	None,		//
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
	FORCEINLINE EEquipedWeapon GetEqipedWeapon(){ return EquipedWeapon; };

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
	UPROPERTY(EditAnywhere, Category = "Eqiped")
	EEquipedWeapon EquipedWeapon = EEquipedWeapon::None;

// Weapon이 부착될 socket
public:
	// Bow
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName BowSocketName = FName("bow_socket_l");
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName ArrowSocketName = FName("arrow_socket_r");
	// Sword
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName SwordSocketName = FName("sword_socket_r");
	// Torch
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName TorchSocketName = FName("torch_socket_l");;
// Weapon
public:
	// 나중에 Map이나 Array로 저장 고려 ]
	// Bow
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AHR_Bow> Weapon_Bow;
	// Sword
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AHR_Sword> Weapon_Sword;
	// Torch
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AHR_Torch> Weapon_Torch;

// Weapon Ins
private:
	// Bow
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class AHR_Bow> BowIns;
	// Sowrd
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class AHR_Sword> SwordIns;
	// Torch
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class AHR_Torch> TorchIns;

// Bow 기능 
public:
	void AimingBowStart();
	void AimingBowEnd();
	void FireBow();

// Weapon eauip
public:
	void EquipWeapon();

// Arrow Direction calculate
private:
	FVector CalculateArrowDirection();
	
};
