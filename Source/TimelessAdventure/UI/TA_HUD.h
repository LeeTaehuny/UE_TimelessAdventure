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
	void SetAimVisibility(bool Value);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_Aim;
};
