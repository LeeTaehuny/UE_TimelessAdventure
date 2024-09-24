// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TA_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API ATA_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATA_PlayerController();

protected:
	virtual void BeginPlay() override;

public:
	void VisibleWeaponSelectWidget(bool Value);

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> WeaponSelectWidgetClass;

	UPROPERTY()
	TObjectPtr<class UTA_ChangeWeapon> WeaponSelectWidget;
};
