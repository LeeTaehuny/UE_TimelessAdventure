// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TA_MainGameMode.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"

ATA_MainGameMode::ATA_MainGameMode()
{
	bCanTeleportPrairie = true;
	bCanTeleportRuins = false;
	bCanTeleportCave = false;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
}

void ATA_MainGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (BGM_Basic && AudioComp)
	{
		AudioComp->SetSound(BGM_Basic);
		AudioComp->Play();
	}
}

void ATA_MainGameMode::StopBGM()
{
	if (AudioComp)
	{
		AudioComp->Stop();
	}
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

void ATA_MainGameMode::SetBoss(AActor* BossActor)
{
	Boss = BossActor;

	if (AudioComp)
	{
		AudioComp->Stop();

		if (BGM_Boss)
		{
			AudioComp->SetSound(BGM_Boss);
			AudioComp->Play();
		}
		
	}
}

void ATA_MainGameMode::DeathBoss()
{
	if (IsValid(Boss))
	{
		Boss->Destroy();
	}

	if (AudioComp)
	{
		AudioComp->Stop();

		if (BGM_Boss)
		{
			AudioComp->SetSound(BGM_Basic);
			AudioComp->Play();
		}
	}
}

FVector ATA_MainGameMode::GetPoint(EMapType MapType)
{
	if (SpawnPoints.Find(MapType))
	{
		return SpawnPoints[MapType];
	}

	return FVector();
}
