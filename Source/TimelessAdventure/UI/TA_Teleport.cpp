// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_Teleport.h"

#include "Components/Button.h"

bool UTA_Teleport::Initialize()
{
	bool Result = Super::Initialize();

	if (BTN_FirstMap) BTN_FirstMap->OnClicked.AddDynamic(this, &UTA_Teleport::TeleportFirstMap);
	if (BTN_SecondMap) BTN_SecondMap->OnClicked.AddDynamic(this, &UTA_Teleport::TeleportSecondMap);
	if (BTN_ThirdMap) BTN_ThirdMap->OnClicked.AddDynamic(this, &UTA_Teleport::TeleportThirdMap);

	return Result;
}

void UTA_Teleport::UpdateTeleport()
{
}

void UTA_Teleport::TeleportFirstMap()
{
}

void UTA_Teleport::TeleportSecondMap()
{
}

void UTA_Teleport::TeleportThirdMap()
{
}
