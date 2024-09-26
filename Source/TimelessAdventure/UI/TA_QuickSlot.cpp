// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_QuickSlot.h"
#include "UI/TA_Slot.h"

void UTA_QuickSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTA_QuickSlot::InitQuickSlot()
{
	S_Hp->SetOwnerPlayer(OwnerActor);
	S_Hp->Init(ESlotType::ST_QuickSlot);

	S_Stamina->SetOwnerPlayer(OwnerActor);
	S_Stamina->Init(ESlotType::ST_QuickSlot);
}

void UTA_QuickSlot::UpdateQuickSlot()
{
	S_Hp->UpdateSlot();
	S_Stamina->UpdateSlot();
}
