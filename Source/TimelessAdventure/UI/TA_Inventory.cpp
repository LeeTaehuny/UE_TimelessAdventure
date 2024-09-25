// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_Inventory.h"
#include "Data/TA_SlotType.h"
#include "UI/TA_Slot.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"

void UTA_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_ChangeC) BTN_ChangeC->OnClicked.AddDynamic(this, &UTA_Inventory::ChangeInventoryType_C);
	if (BTN_ChangeM) BTN_ChangeM->OnClicked.AddDynamic(this, &UTA_Inventory::ChangeInventoryType_M);

	TArray<UWidget*> TempWidgets;
	WidgetTree->GetAllWidgets(TempWidgets);

	for (UWidget* Widget : TempWidgets)
	{
		if (Widget->GetName().Contains(TEXT("Slot")))
		{
			UTA_Slot* InvSlot = Cast<UTA_Slot>(Widget);
			if (InvSlot)
			{
				InvSlot->SetOwnerPlayer(OwnerActor);
				Slots.Push(InvSlot);
			}
		}
	}

	InventoryType = EInventoryType::IT_Consum;
}

void UTA_Inventory::InitInventory()
{
	// 슬롯 초기화
	UpdateInvenSlot();
}

void UTA_Inventory::ChangeInventoryType_C()
{
	UE_LOG(LogTemp, Warning, TEXT("C"));
	InventoryType = EInventoryType::IT_Consum;
	UpdateInvenSlot();
}

void UTA_Inventory::ChangeInventoryType_M()
{
	UE_LOG(LogTemp, Warning, TEXT("M"));
	InventoryType = EInventoryType::IT_Misc;
	UpdateInvenSlot();
}

void UTA_Inventory::UpdateInvenSlot()
{
	// 슬롯 초기화
	for (UTA_Slot* InvSlot : Slots)
	{
		switch (InventoryType)
		{
		case EInventoryType::IT_Consum:
			InvSlot->Init(ESlotType::ST_Inventory_C);
			break;
		case EInventoryType::IT_Misc:
			InvSlot->Init(ESlotType::ST_Inventory_M);
			break;
		default:
			break;
		}
		
		InvSlot->UpdateSlot();
	}
}