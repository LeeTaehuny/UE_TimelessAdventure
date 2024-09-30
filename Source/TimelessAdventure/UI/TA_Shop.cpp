// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_Shop.h"
#include "UI/TA_Slot.h"
#include "Data/TA_SlotType.h"
#include "Interface/InventoryInterface.h"
#include "Component/TA_InventoryComponent.h"
#include "Gimmick/TA_Stone.h"

#include "GameFramework/Character.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UTA_Shop::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UTA_Shop::Initialize()
{
	bool Result = Super::Initialize();

	if (!BTN_Purchase1 || !BTN_Purchase2) return false;

	BTN_Purchase1->OnClicked.AddDynamic(this, &UTA_Shop::PurchaseItem1);
	BTN_Purchase2->OnClicked.AddDynamic(this, &UTA_Shop::PurchaseItem2);

	return Result;
}

void UTA_Shop::Init()
{
	Slot1->SetOwnerPlayer(OwnerActor);
	Slot1->Init(ESlotType::ST_Shop);

	Slot2->SetOwnerPlayer(OwnerActor);
	Slot2->Init(ESlotType::ST_Shop);

	InitItemDesc();
	UpdateGold();
}

void UTA_Shop::UpdateGold()
{
	IInventoryInterface* InvInterface = Cast<IInventoryInterface>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (InvInterface)
	{
		int32 Gold = InvInterface->GetInventory()->GetGold();
		TXT_Gold->SetText(FText::FromString(FString::FromInt(Gold)));
	}

	if (ATA_Stone* Stone = Cast<ATA_Stone>(OwnerActor))
	{
		// 아이템 가격이 현재 소지 골드보다 더 비싼 경우
		if (Stone->GetShopItemData()[0].Price > InvInterface->GetInventory()->GetGold())
		{
			// 해당 버튼 비활성화
			BTN_Purchase1->SetIsEnabled(false);
		}
		else
		{
			BTN_Purchase1->SetIsEnabled(true);
		}

		// 아이템 가격이 현재 소지 골드보다 더 비싼 경우
		if (Stone->GetShopItemData()[1].Price > InvInterface->GetInventory()->GetGold())
		{
			// 해당 버튼 비활성화
			BTN_Purchase2->SetIsEnabled(false);
		}
		else
		{
			BTN_Purchase2->SetIsEnabled(true);
		}
	}
}

void UTA_Shop::InitItemDesc()
{
	if (ATA_Stone* Stone = Cast<ATA_Stone>(OwnerActor))
	{
		TXT_ItemName1->SetText(Stone->GetShopItemData()[0].ItemName);
		TXT_ItemName2->SetText(Stone->GetShopItemData()[1].ItemName);

		TXT_ItemDesc1->SetText(Stone->GetShopItemData()[0].ItemDescription);
		TXT_ItemDesc2->SetText(Stone->GetShopItemData()[1].ItemDescription);

		TXT_ItemPrice1->SetText(FText::FromString(FString::FromInt(Stone->GetShopItemData()[0].Price)));
		TXT_ItemPrice2->SetText(FText::FromString(FString::FromInt(Stone->GetShopItemData()[1].Price)));
	}
}

void UTA_Shop::PurchaseItem1()
{
	if (ATA_Stone* Stone = Cast<ATA_Stone>(OwnerActor))
	{
		IInventoryInterface* InvInterface = Cast<IInventoryInterface>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (InvInterface)
		{
			InvInterface->GetInventory()->PurchaseItem(Stone->GetShopItemName()[0], Stone->GetShopItemData()[0].Price);
			UpdateGold();
		}
	}
}

void UTA_Shop::PurchaseItem2()
{
	if (ATA_Stone* Stone = Cast<ATA_Stone>(OwnerActor))
	{
		IInventoryInterface* InvInterface = Cast<IInventoryInterface>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (InvInterface)
		{
			InvInterface->GetInventory()->PurchaseItem(Stone->GetShopItemName()[1], Stone->GetShopItemData()[1].Price);
			UpdateGold();
		}
	}
}
