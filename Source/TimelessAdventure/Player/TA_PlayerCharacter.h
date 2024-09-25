﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/AnimUpdateInterface.h"
#include "../Interface/PlayerComponentInterface.h"
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
	// 이건 왜 virtual ??
	FORCEINLINE virtual class UTA_CombatComponent* GetCombatComponent() override { return TA_CombatComponent; }
	FORCEINLINE class UTA_InputComponent* GetInputComponent() { return TA_InputComponent; }

	FORCEINLINE void SetHasBow(bool b) { bHasBow = b; };
	FORCEINLINE bool GetHasBow() const { return bHasBow; };
	FORCEINLINE void SetAimingBow(bool b) { bisAimingBow = b; };
	FORCEINLINE bool GetAimingBow() const { return bisAimingBow; };
	FORCEINLINE void SetZooming(bool b) { bisZooming = b; };
	FORCEINLINE bool GetZooming() const { return bisZooming; };
	
// Bow
private:
	bool bHasBow = true;
	bool bisAimingBow;
	bool bisZooming = false;

// Zoom
public:
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float InitialFOV;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float AimFOV = 60.0f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	FVector InitialSO;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	FVector AimSO = FVector(60, 60, 45);
	

// Weapon BP
protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AHR_Bow> Weapon_Bow;


	
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
