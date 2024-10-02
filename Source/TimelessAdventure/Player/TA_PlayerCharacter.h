// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AnimUpdateInterface.h"
#include "Interface/CombatComponentInterface.h"
#include "Interface/InventoryInterface.h"
#include "TA_PlayerCharacter.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_PlayerCharacter : public ACharacter,
	public ICombatComponentInterface, public IInventoryInterface
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
	FORCEINLINE virtual class UTA_InventoryComponent* GetInventory() override { return TA_InventoryComponent; }
	
	FORCEINLINE class UTA_InputComponent* GetInputComponent() { return TA_InputComponent; }
	FORCEINLINE class UTA_InventoryComponent* GetInventoryComponent() { return TA_InventoryComponent; }
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() { return SpringArmComp; }
	FORCEINLINE class UHR_StopAbilityComponent* GetStopAbilityComponent() {return TA_StopAbilityComponent; }
	FORCEINLINE class USceneComponent* GetRootComponent() { return RootComponent; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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
	TObjectPtr<class UTA_InventoryComponent> TA_InventoryComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UHR_StopAbilityComponent> TA_StopAbilityComponent;
};
