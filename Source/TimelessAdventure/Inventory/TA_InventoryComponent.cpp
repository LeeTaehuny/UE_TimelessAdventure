// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"


UTA_InventoryComponent::UTA_InventoryComponent()
{
	MaxInventorySize = 30;
}

FItemData UTA_InventoryComponent::LoadItemFromDataTable(FName ItemRowName)
{
	if(!ItemDataTable)
	{
		return FItemData();
	}
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemRowName, TEXT("ItemLookup"));
	if(ItemData)
	{
		return *ItemData;
	}
	else
	{
		return FItemData();
	}
}


void UTA_InventoryComponent::AddItemToInventory(FName ItemRowName, int32 Quantity)
{
	if(IsInventoryFull())
	{
		return;
	}
	if(Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ExistingItem = Inventory[ItemRowName];
		if(ExistingItem.ItemData.bIsStackable)
		{
			ExistingItem.Quantity += Quantity;
		}
		else
		{
			
		}
	}
	else
	{
		if(ItemDataTable)
		{
			FItemData ItemData = LoadItemFromDataTable(ItemRowName);
			FInventoryItem NewItem;
			NewItem.ItemData = ItemData;
			NewItem.Quantity = Quantity;
			Inventory.Add(ItemRowName, NewItem);
		}
	}
}
bool UTA_InventoryComponent::IsInventoryFull() const
{
	return Inventory.Num() > 0;
}

void UTA_InventoryComponent::RemoveItemFromInventory(FName ItemRowName, int32 Quantity)
{
	if(Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ItemData = Inventory[ItemRowName];
		ItemData.Quantity -= Quantity;
		if(ItemData.Quantity <= 0)
		{
			Inventory.Remove(ItemRowName);
		}
		else
		{
			
		}
	}
	else
	{
		
	}
}

void UTA_InventoryComponent::ToggleInventory()
{

	AActor* Owner = GetOwner();
	if(!Owner)
	{

		return;
	}	
	ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
	if(!OwnerCharacter)
	{
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if(PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController"));
		if(!PlayerInventoryWidget && PlayerInventoryWidgetClass)
		{
			PlayerInventoryWidget = CreateWidget<UUserWidget>(PlayerController, PlayerInventoryWidgetClass);
			if(PlayerInventoryWidget)
			{
				PlayerInventoryWidget->AddToViewport();
				PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
				
			}
		}
		if(PlayerInventoryWidget)
		{
			if(bIsInventoryVisible)
			{
				PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
				bIsInventoryVisible = false;

				FInputModeGameOnly InputMode;
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = false;
			}
			else
			{
				PlayerInventoryWidget->SetVisibility(ESlateVisibility::Visible);
				bIsInventoryVisible = true;
				FInputModeGameAndUI InputMode;
				InputMode.SetWidgetToFocus(PlayerInventoryWidget->TakeWidget());
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = true;
			}
		}
	}
}
