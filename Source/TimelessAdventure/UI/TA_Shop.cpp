// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_Shop.h"
#include "UI/TA_Slot.h"
#include "Data/TA_SlotType.h"
#include "Interface/InventoryInterface.h"
#include "Component/TA_InventoryComponent.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UTA_Shop::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTA_Shop::Init()
{
	Slot1->SetOwnerPlayer(OwnerActor);
	Slot1->Init(ESlotType::ST_Shop);

	Slot2->SetOwnerPlayer(OwnerActor);
	Slot2->Init(ESlotType::ST_Shop);

	UpdateGold();
}

void UTA_Shop::UpdateGold()
{
	IInventoryInterface* InvInterface = Cast<IInventoryInterface>(OwnerActor);
	if (InvInterface)
	{
		int32 Gold = InvInterface->GetInventory()->GetGold();
		TXT_Gold->SetText(FText::FromString(FString::FromInt(Gold)));
	}
}
