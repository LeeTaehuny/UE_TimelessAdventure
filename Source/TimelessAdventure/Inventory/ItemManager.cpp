// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"
#include "Engine/DataTable.h"
#include "Misc/Optional.h"

UItemManager::UItemManager()
{
	
}


void UItemManager::LoadItemData(UDataTable* ItemDataTable)
{
	if(ItemDataTable)
	{
		FString ContextString;
		TArray<FItemData*> AllItems;
		ItemDataTable->GetAllRows(ContextString, AllItems);

		for(FItemData* Item : AllItems)
		{
			ItemDataMap.Add(Item->ItemID, *Item);
			UE_LOG(LogTemp, Warning, TEXT("Item loaded : %s"), *Item->ItemName.ToString());
		}
	}
}

FItemData& UItemManager::GetItemDataByID(int32 ItemID)
{
	if(ItemDataMap.Contains(ItemID))
	{
		return ItemDataMap[ItemID];
	}
	return ItemDataMap[0];
}

TOptional<FItemData> UItemManager::GetItemDataByIDOptional(int32 ItemID)
{
	if(ItemDataMap.Contains(ItemID))
	{
		return ItemDataMap[ItemID];
	}
	return TOptional<FItemData>();
}

