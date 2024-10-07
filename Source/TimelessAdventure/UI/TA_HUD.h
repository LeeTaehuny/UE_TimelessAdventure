// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_HUD : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	bool GetInventoryVisibility();
	void SetAimVisibility(bool Value);
	void SetInventoryVisibility(bool Value);
	void SetInteractionVisibility(bool Value);
	void SetInteractionText(FText NewText);
	void Init();
	void UpdateInventory();
	void UpdateGold();

// prograssbar 함수
public:
	void SetPBPercentHealth(float Percent);
	void SetPBPercentStamina(float Percent);
	void SetPBPercentTimeEnergy(float Percent);
	

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_Aim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_Inventory> InventoryWidget	;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_QuickSlot> QuickSlotWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_Interaction> InteractionWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_Health;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_Stamina;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_TimeEnergy;
	
};
