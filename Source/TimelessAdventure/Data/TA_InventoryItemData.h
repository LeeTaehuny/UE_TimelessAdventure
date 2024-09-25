// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/TA_ItemData.h"
#include "TA_InventoryItemData.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_InventoryItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FItemData ItemData;

	UPROPERTY(VisibleAnywhere)
	int32 ItemQuantity;
};
