// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AnimUpdateInterface.h"
#include "Interface/PlayerComponentInterface.h"
#include "TA_PlayerCharacter.generated.h"



UCLASS()
class TIMELESSADVENTURE_API ATA_PlayerCharacter : public ACharacter, public IAnimUpdateInterface, public IPlayerComponentInterface
{
	GENERATED_BODY()

public:
	ATA_PlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

// Getter
public:
	FORCEINLINE virtual class UTA_CombatComponent* GetCombatComponent() override { return TA_CombatComponent; }
	FORCEINLINE void SetHasBow(bool b) { bHasBow = b; };
	FORCEINLINE bool GetHasBow() const { return bHasBow; };
	FORCEINLINE void SetAimingBow(bool b) { bAimingBow = b; };
	FORCEINLINE bool GetAimingBow() const { return bAimingBow; };
private:
	bool bHasBow = true;
	bool bAimingBow;
	
// Camera Component
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComp;

// Components
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UTA_InputComponent> TA_InputComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UTA_CombatComponent> TA_CombatComponent;
};
