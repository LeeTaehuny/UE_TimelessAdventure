// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_PlayerComponentBase.h"
#include "InputActionValue.h"
#include "TA_InputComponent.generated.h"

// 현재 플레이어의 상태를 구분하기 위한 열거형
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	PS_Walk,	// normal(walk)
	PS_Dash,	// run
	PS_Combat,  // combat
	PS_Roll,	// roll
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

	// function for input
	void AddInput(UInputComponent* PlayerInputComponent);

// Input Mapping Context & Input Action
protected:
	void BasicMove(const FInputActionValue& Value);	// player move
	void BasicLook(const FInputActionValue& Value);	// Look
	void DashStart();								// sprint start
	void DashEnd();									// sprint end
	void BasicRoll();								// roll
	void BasicJump();								// jumping

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

// Animations
protected:
	// roll AminMontage
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> RollMontage;

	// called function when finished playing RollMontage
	void OnRollMontageEnd(class UAnimMontage* Montage, bool bInterrupted);

// Member
private:

	// change state function
	void ChangeState(EPlayerState NewState);

	// player status
	UPROPERTY(VisibleAnywhere, Category = "State")
	EPlayerState PlayerState;

	// walk speed
	UPROPERTY(EditAnywhere, Category = "Settings")
	float WalkSpeed;

	// run speed
	UPROPERTY(EditAnywhere, Category = "Settings")
	float DashSpeed;

	// variable for saving temp state
	EPlayerState TempState;
	// variable for saving temp movement value
	FVector2D MovementVector;

	// Stamina for roll
	UPROPERTY(EditAnywhere, Category = "Settings")
	float RollStaminaPercent;
};
