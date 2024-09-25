// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/TA_ItemData.h"
#include "TA_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTA_GameInstance();

public:
	FItemData GetItemData(FName ItemName);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<class UDataTable> ItemDataTable;
};
