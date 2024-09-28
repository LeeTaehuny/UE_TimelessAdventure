// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_Shop.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_Shop : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init();
	void UpdateGold();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Gold;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_Slot> Slot1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_Slot> Slot2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemName1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemName2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemDesc1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemDesc2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemPrice1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemPrice2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Purchase1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Purchase2;
};
