// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_QuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_QuickSlot : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void InitQuickSlot();
	void UpdateQuickSlot();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_Slot> S_Hp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTA_Slot> S_Stamina;
};
