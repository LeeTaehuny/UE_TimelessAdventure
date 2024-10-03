// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/TA_PlayerComponentBase.h"
#include "Blueprint/UserWidget.h"
#include "TA_GrapRotateComponent.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_GrapRotateComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()
public:
	void CreateWidget() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> WidgetClass;
};
