// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_CustomWidget.h"
#include "HR_StateChange.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UHR_StateChange : public UTA_CustomWidget
{
	GENERATED_BODY()

// override event
protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

// 바인딩 함수
private:
	UFUNCTION()
	void OnClickedCombat();
	UFUNCTION()
	void OnClickedStopObject();
	UFUNCTION()
	void OnClickedGrapObject();

// widget object
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Combat;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_StopObject;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_GrapObject;
	
};
