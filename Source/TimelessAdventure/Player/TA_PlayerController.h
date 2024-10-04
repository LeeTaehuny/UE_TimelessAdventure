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
	void ConvertInventoryWidget();
	void VisibleAimWidget(bool Value);
	void VisibleInteractionWidget(bool Value);
	void SetInteractionText(FText NewText);
	bool GetInventoryVisible();


private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> WeaponSelectWidgetClass;

	UPROPERTY()
	TObjectPtr<class UTA_ChangeWeapon> WeaponSelectWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UTA_HUD> HUDWidget;


};
