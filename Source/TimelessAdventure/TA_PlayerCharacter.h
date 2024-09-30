// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_WeaponComponent_sword.h"
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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UTA_WeaponComponent> TA_WeaponComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UTA_WeaponComponent_sword> TA_WeaponComponent_Sword;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UTA_WeaponComponent_bow> TA_WeaponComponent_Bow;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UTA_WeaponComponent_torch> TA_WeaponComponent_Torch;
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<class UTA_InteractionComponent> TA_InteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	class UTA_InventoryComponent* DefaultInventory;
	
};
