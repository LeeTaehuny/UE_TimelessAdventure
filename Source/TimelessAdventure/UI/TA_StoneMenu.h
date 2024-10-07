// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_StoneMenu.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_StoneMenu : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

public:
	void Init();
	void UpdateWidget();

private:
	UFUNCTION()
	void SwitchShop();

	UFUNCTION()
	void SwitchTeleport();

	UFUNCTION()
	void CloseWidget();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SelectShop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SelectTeleport;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Close;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> WS_Menu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_Shop> Shop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_Teleport> Teleport;
};
