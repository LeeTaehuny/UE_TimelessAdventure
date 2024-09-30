// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "TA_InventoryComponent.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;
	FInventoryItem()
		:ItemData(), Quantity(1)
	{}
};
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMELESSADVENTURE_API UTA_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTA_InventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, FInventoryItem> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemData LoadItemFromDataTable(FName ItemRowName);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(FName ItemRowName, int32 Quantity);
	bool IsInventoryFull() const;
	void RemoveItemFromInventory(FName ItemRowName, int32 Quantity);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySize;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PlayerInventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UUserWidget* PlayerInventoryWidget;
	bool bIsInventoryVisible;

	void ToggleInventory();
};
