// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiffResults.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UItemManager : public UObject
{
	GENERATED_BODY()
public:
	UItemManager();

	TMap<int32, FItemData>ItemDataMap;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void LoadItemData(UDataTable* ItemDataTable);

	UFUNCTION(BlueprintCallable, Category = "Item")
	TOptional<FItemData> GetItemDataByIDOptional(int32 ItemID);

	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData& GetItemDataByID(int32 ItemID);
};

