// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_Teleport : public UTA_CustomWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
public:
	void UpdateTeleport();

protected:
	UFUNCTION()
	void TeleportFirstMap();

	UFUNCTION()
	void TeleportSecondMap();

	UFUNCTION()
	void TeleportThirdMap();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_FirstMap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SecondMap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_ThirdMap;
};
