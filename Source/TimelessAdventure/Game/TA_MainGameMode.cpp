// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TA_MainGameMode.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ATA_MainGameMode::ATA_MainGameMode()
{
	bCanTeleportPrairie = true;
	bCanTeleportRuins = false;
	bCanTeleportCave = false;
}

void ATA_MainGameMode::RespawnPlayer()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player)
	{
		Player->SetActorLocation(RespawnLocation);
	}
}

void ATA_MainGameMode::TeleportPlayer(EMapType MapType)
{
	if (SpawnPoints.Find(MapType))
	{
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (!Player) return;

		Player->SetActorLocation(SpawnPoints[MapType]);

		// 리스폰 위치 저장
		SetRespawnLocation(SpawnPoints[MapType]);
	}
}

void ATA_MainGameMode::AddPoint(EMapType MapType, FVector NewLocation)
{
	SpawnPoints.Add({ MapType, NewLocation });
}

void ATA_MainGameMode::SetRespawnLocation(FVector NewLocation)
{
	RespawnLocation = NewLocation;
}

FVector ATA_MainGameMode::GetPoint(EMapType MapType)
{
	if (SpawnPoints.Find(MapType))
	{
		return SpawnPoints[MapType];
	}

	return FVector();
}
