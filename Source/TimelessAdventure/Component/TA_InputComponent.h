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

	PS_Combat,			// 전투
	PS_StopAbilityMode,		// 정지 능력 모드


	PS_Gimmick

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMELESSADVENTURE_API UTA_InputComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()

public:
	UTA_InputComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 입력을 전달받기 위한 함수
	void AddInput(UInputComponent* PlayerInputComponent);

public:
	// 상태 변경 함수
	void ChangeState(EPlayerState NewState);

// Input Mapping Context & Input Action
protected:
	void BasicMove(const FInputActionValue& Value);	// 플레이어 이동
	void BasicLook(const FInputActionValue& Value);	// 시야 회전
	void DashStart();								// 달리기 시작
	void DashEnd();									// 달리기 종료
	void BasicRoll();								// 구르기
	void BasicJump();								// 점프
	void LeftClick();								// 좌클릭
	void RightClickStart();							// 우클릭 시작
	void RightClickEnd();							// 우클릭 종료
	void MiddleClickStart();						// 중간 버튼 클릭 시작
	void MiddleClickEnd();							// 중간 버튼 클릭 종료
	void ConvertInventory();						// 인벤토리 On/Off
	void NumClick(int32 ClickNum);					// 숫자 키보드 입력
	void Interaction();								// 상호작용
	// hr
	void TabClick();								// 공격, 플레이어 기능 (오브젝트 정지, 오브젝트 control)
	

	void Grap();									// 그랩
	void SwitchState();
	void Rotate(const FInputActionValue& Value);
	void SetX();
	void SetZ();
	void SetXfalse();
	void SetZfalse();
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

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_LeftClick;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_RightClick;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_MiddleClick;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_ConvertInventory;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Num1;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Num2;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Interaction;

	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Tab;

	TObjectPtr<class UInputAction> IA_Grap;
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Switch;
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_Rotate;
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_z;
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<class UInputAction> IA_x;
	
// Member
private:
	// 플레이어의 현재 상태
	UPROPERTY(VisibleAnywhere, Category = "State")
	EPlayerState PlayerState;

	// 임시 이동 입력값 저장용 변수
	FVector2D MovementVector;
};
