﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AnimUpdateInterface.h"
#include "TA_PlayerCharacter.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_PlayerCharacter : public ACharacter, public IAnimUpdateInterface
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
	FORCEINLINE class UTA_CombatComponent* GetCombatComponent() { return TA_CombatComponent; }
	FORCEINLINE class UTA_InputComponent* GetInputComponent() { return TA_InputComponent; }

	virtual bool GetIsAttacking() override;

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
