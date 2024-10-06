// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_StoneMenu.h"
#include "UI/TA_Shop.h"
#include "UI/TA_Teleport.h"
#include "Gimmick/TA_Stone.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void UTA_StoneMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// 기본 이미지로 설정
	WS_Menu->SetActiveWidgetIndex(0);
}

bool UTA_StoneMenu::Initialize()
{
	bool Result = Super::Initialize();

	if (!BTN_SelectShop || !BTN_SelectTeleport || !BTN_Close) return false;

	// 클릭 이벤트 설정
	BTN_SelectShop->OnClicked.AddDynamic(this, &UTA_StoneMenu::SwitchShop);
	BTN_SelectTeleport->OnClicked.AddDynamic(this, &UTA_StoneMenu::SwitchTeleport);
	BTN_Close->OnClicked.AddDynamic(this, &UTA_StoneMenu::CloseWidget);

	return Result;
}

void UTA_StoneMenu::Init()
{
	Shop->SetOwnerPlayer(OwnerActor);
	Shop->Init();
}

void UTA_StoneMenu::UpdateWidget()
{
	Shop->UpdateGold();
}

void UTA_StoneMenu::SwitchShop()
{
	if (Shop)
	{
		WS_Menu->SetActiveWidget(Shop);
	}
}

void UTA_StoneMenu::SwitchTeleport()
{
	if (Teleport)
	{
		Teleport->UpdateTeleport();
		WS_Menu->SetActiveWidget(Teleport);
	}
}

void UTA_StoneMenu::CloseWidget()
{
	// TODO
	if (ATA_Stone* Stone = Cast<ATA_Stone>(OwnerActor))
	{
		Stone->SetVisibilityStoneMenu(false);
	}
}
