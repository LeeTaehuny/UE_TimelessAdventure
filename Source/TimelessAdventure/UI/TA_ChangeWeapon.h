// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_ChangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_ChangeWeapon : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void OpenWidget();

private:
	UFUNCTION()
	void OnHoverFist();

	UFUNCTION()
	void OnHoverSword();

	UFUNCTION()
	void OnHoverBow();

	UFUNCTION()
	void OnHoverTorch();

	UFUNCTION()
	void OnHoverCancel();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> OL_Main;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Fist;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Sword;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Bow;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Torch;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Cancel;
};
