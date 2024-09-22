﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_PlayerComponentBase.h"
#include "InputActionValue.h"
#include "TA_InputComponent.generated.h"

// 현재 플레이어의 상태를 구분하기 위한 열거형
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	PS_Idle,	// 
	PS_Walk,	// 일반(걷기)
	PS_Dash,	// 달리기
	PS_Combat,  // 전투
	PS_Roll,	// 구르기
	PS_Jump,	// 점프
};

// 무기 Enum
// 이걸 여기에 두는 게 맞을까? character가 가지고 있어야 하지 않을까?
UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	WT_Body,		// 맨몸
	WT_Sword,		// 칼
	WT_Bow,			// 활
	WT_Torch,		// 토치
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMELESSADVENTURE_API UTA_InputComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()

public:
	UTA_InputComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 입력을 전달받기 위한 함수
	void AddInput(UInputComponent* PlayerInputComponent);

// Input Mapping Context & Input Action
protected:
	void BasicMove(const FInputActionValue& Value);	// 플레이어 이동
	void BasicLook(const FInputActionValue& Value);	// 시야 회전
	void DashStart();								// 달리기 시작
	void DashEnd();									// 달리기 종료
	void BasicRoll();								// 구르기
	void BasicJump();								// 점프

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputMappingContext> IMC_Player;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Jump;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Roll;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Dash;

	// Attack Combo Input Action (by hr)
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_SwordAttack;
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_BowAttack;

	// 공격 바인딩 함수 (by hr)
	// Sword Attack	
	void SwordAttack();
	// BowAttack
	void BowAttackStart();
	void BowAttackEnd();
	// Notify handle
	UFUNCTION()
	void HandleOnMontageSwordAttackNotifyBegin(FName notifyName, const FBranchingPointNotifyPayload& branchingpayload);

// Animations
protected:
	// 구르기 AminMontage
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> RollMontage;

	// RollMontage가 종료되면 호출되는 함수
	void OnRollMontageEnd(class UAnimMontage* Montage, bool bInterrupted);

	// Anim Montage (by hr)
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> SwordComboAttackMontage;

// Member
private:
	// 플레이어 현재 weapone
	UPROPERTY(EditAnywhere, Category = "Settings")
	EWeaponType WeaponType;

	// 상태 변경 함수
	void ChangeState(EPlayerState NewState);

	// 플레이어의 현재 상태
	UPROPERTY(VisibleAnywhere, Category = "State")
	EPlayerState PlayerState;

	// 걷기 속도
	UPROPERTY(EditAnywhere, Category = "Settings")
	float WalkSpeed;

	// 달리기 속도
	UPROPERTY(EditAnywhere, Category = "Settings")
	float DashSpeed;

	// 임시 상태 저장용 변수
	EPlayerState TempState;
	// 임시 이동 입력값 저장용 변수
	FVector2D MovementVector;

	// 구르기에 사용될 체력 퍼센트
	UPROPERTY(EditAnywhere, Category = "Settings")
	float RollHealthPercent;

	// combo 공격을 위한 index (by hr)
	int32 SwordComboIndex = 0;
	// 자주 사용해서 변수로 만들어둠
	class UAnimInstance* animInstance;


private:
	// 공격 확인 하는 함수
	bool IsAttacking();
	bool isAttack;
};
