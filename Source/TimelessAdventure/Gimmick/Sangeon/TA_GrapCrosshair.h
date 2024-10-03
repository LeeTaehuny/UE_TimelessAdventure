// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "TA_GrapCrosshair.generated.h"

/**
 * 
 */
class UImage;
UCLASS()
class TIMELESSADVENTURE_API UTA_GrapCrosshair : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Crosshair;
	
public:
	UFUNCTION(BlueprintCallable)
	void UnHover();

	UFUNCTION(BlueprintCallable)
	void Hover();

	UPROPERTY(BlueprintReadWrite, Category = "Hover")
	bool IsHover;


};
