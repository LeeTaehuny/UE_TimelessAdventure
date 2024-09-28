// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_PlayerComponentBase.h"
#include "Data/TA_InventoryItemData.h"
#include "Data/TA_SlotType.h"
#include "TA_InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangeInventory);

USTRUCT(BlueprintType)
struct FInvItem
{
	GENERATED_BODY()

public:
	FInvItem() : bIsEmpty(true) {}

public:
	FItemData Data;
	int32 Quantity;
	bool bIsEmpty;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TIMELESSADVENTURE_API UTA_InventoryComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()

public:
	UTA_InventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	FOnChangeInventory OnChangeInventory;

	FORCEINLINE const TArray<FInvItem>& GetCInventory() { return Inventory_C; }
	FORCEINLINE const TArray<FInvItem>& GetMInventory() { return Inventory_M; }
	FORCEINLINE const TArray<int32>& GetQuickSlot() { return QuickSlot; }
	FORCEINLINE const int32 GetGold() { return Gold; }

public:
	void ConvertInventory();
	
	bool AddItem(FName ItemName, int32& Quantity);
	void UseQuickSlot(int32 Num);
	void UseItem(ESlotType Type, int32 Index);
	void RemoveItem(ESlotType Type, int32 Index);
	void SwapItem(ESlotType Type1, int32 Index1, ESlotType Type2, int32 Index2);

	void AddQuickSlot(ESlotType Type, int32 Index1, int32 Index2);

private:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FInvItem> Inventory_C;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FInvItem> Inventory_M;

	UPROPERTY()
	TArray<int32> QuickSlot;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 Gold;
};
