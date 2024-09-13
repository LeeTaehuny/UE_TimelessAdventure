// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "TA_InputComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	PS_Idle,	// �Ϲ�
	PS_Walk,	// �ȱ�
	PS_Dash,	// �޸���
	PS_Ghost,	// ����
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMELESSADVENTURE_API UTA_InputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTA_InputComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE void SetOwnerPlayer(ACharacter* InPlayer) { OwnerPlayer = InPlayer; }

	void AddInput(UInputComponent* PlayerInputComponent);

// Input Mapping Context & Input Action
protected:
	void BasicMove(const FInputActionValue& Value);
	void BasicLook(const FInputActionValue& Value);
	void DashStart();
	void DashEnd();
	void BasicRoll();

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
	UPROPERTY(EditAnywhere, Category = "Anims")
	TObjectPtr<class UAnimMontage> RollMontage;

// Owner Player
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> OwnerPlayer;

// Member
private:
	void ChangeState(EPlayerState NewState);

	UPROPERTY(VisibleAnywhere, Category = "State")
	EPlayerState PlayerState;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
	float DashSpeed;
};
