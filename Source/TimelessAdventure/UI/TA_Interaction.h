// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_Interaction.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_Interaction : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHelpText(FText NewText);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_HelpText;
};
