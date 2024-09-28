// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_HUD.h"
#include "UI/TA_Inventory.h"
#include "UI/TA_QuickSlot.h"
#include "UI/TA_Interaction.h"

#include "Components/Image.h"

void UTA_HUD::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UTA_HUD::GetInventoryVisibility()
{
	if (InventoryWidget)
	{
		return InventoryWidget->IsVisible();
	}
	return false;
}

void UTA_HUD::SetAimVisibility(bool Value)
{
	IMG_Aim->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UTA_HUD::SetInventoryVisibility(bool Value)
{
	InventoryWidget->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UTA_HUD::SetInteractionVisibility(bool Value)
{
	InteractionWidget->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UTA_HUD::SetInteractionText(FText NewText)
{
	InteractionWidget->UpdateHelpText(NewText);
}

void UTA_HUD::Init()
{
	if (InventoryWidget)
	{
		InventoryWidget->SetOwnerPlayer(OwnerActor);
		InventoryWidget->InitInventory();
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (QuickSlotWidget)
	{
		QuickSlotWidget->SetOwnerPlayer(OwnerActor);
		QuickSlotWidget->InitQuickSlot();
	}

	if (InteractionWidget)
	{
		InteractionWidget->SetOwnerPlayer(OwnerActor);
		InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTA_HUD::UpdateInventory()
{
	if (InventoryWidget)
	{
		InventoryWidget->UpdateInvenSlot();
	}

	if (QuickSlotWidget)
	{
		QuickSlotWidget->UpdateQuickSlot();
	}
}

void UTA_HUD::UpdateGold()
{
	InventoryWidget->UpdateGold();
}
