// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_BossHp.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_BossHp : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetPBPercentHealth(float Percent);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_BossHp;
};
