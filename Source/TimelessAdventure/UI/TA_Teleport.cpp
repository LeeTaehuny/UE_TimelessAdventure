// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_Teleport.h"
#include "Data/TA_MapType.h"
#include "Game/TA_MainGameMode.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UTA_Teleport::Initialize()
{
	bool Result = Super::Initialize();

	if (BTN_FirstMap) BTN_FirstMap->OnClicked.AddDynamic(this, &UTA_Teleport::TeleportFirstMap);
	if (BTN_SecondMap) BTN_SecondMap->OnClicked.AddDynamic(this, &UTA_Teleport::TeleportSecondMap);
	if (BTN_ThirdMap) BTN_ThirdMap->OnClicked.AddDynamic(this, &UTA_Teleport::TeleportThirdMap);

	return Result;
}

void UTA_Teleport::Init()
{
	BTN_FirstMap->SetIsEnabled(false);
	BTN_SecondMap->SetIsEnabled(false);
	BTN_ThirdMap->SetIsEnabled(false);
}

void UTA_Teleport::UpdateTeleport()
{
	ATA_MainGameMode* GM = Cast<ATA_MainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		if (GM->GetTeleportPrairie()) BTN_FirstMap->SetIsEnabled(true);
		if (GM->GetTeleportRuins()) BTN_SecondMap->SetIsEnabled(true);
		if (GM->GetTeleportCave()) BTN_ThirdMap->SetIsEnabled(true);
	}
}

void UTA_Teleport::TeleportFirstMap()
{
	ATA_MainGameMode* GM = Cast<ATA_MainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->TeleportPlayer(EMapType::MT_Prairie);
	}
}

void UTA_Teleport::TeleportSecondMap()
{
	ATA_MainGameMode* GM = Cast<ATA_MainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->TeleportPlayer(EMapType::MT_Ruins);
	}
}

void UTA_Teleport::TeleportThirdMap()
{
	ATA_MainGameMode* GM = Cast<ATA_MainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->TeleportPlayer(EMapType::MT_Cave);
	}
}
