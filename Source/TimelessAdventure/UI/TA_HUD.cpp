// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_HUD.h"

#include "Components/Image.h"
#include "UI/TA_Inventory.h"

void UTA_HUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTA_HUD::SetAimVisibility(bool Value)
{
	IMG_Aim->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UTA_HUD::Init()
{
	if (InventoryWidget)
	{
		InventoryWidget->SetOwnerPlayer(OwnerActor);
		InventoryWidget->InitInventory();
	}
}

void UTA_HUD::UpdateInventory()
{
	if (InventoryWidget)
	{
		InventoryWidget->UpdateInvenSlot();
	}
}