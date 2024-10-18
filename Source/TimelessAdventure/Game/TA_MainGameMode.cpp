// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TA_MainGameMode.h"

#include "EditorDirectories.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
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
	//AddPoint(EMapType::MT_Cave, FVector(700.0f, 800.0f, 900.0f));
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

	//if(SpawnPoints.Find(MapType))
	//{
		//MapType_temp = MapType;

		if (levelToLoad.IsValid())
		{
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			LatentInfo.ExecutionFunction = FName("OnLevelLoaded");
			LatentInfo.Linkage = 0;
			LatentInfo.UUID = __LINE__;

			UGameplayStatics::LoadStreamLevel(this, levelToLoad, true, true, LatentInfo);
		}
		else
		{
			
		}
	
}
void ATA_MainGameMode::OnLevelLoaded()
{

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player && SpawnPoints.Find(MapType_temp))
	{
		FVector SpawnLocation = SpawnPoints[MapType_temp];
		Player->SetActorLocation(SpawnLocation);
		SetRespawnLocation(SpawnLocation);
        
		UE_LOG(LogTemp, Log, TEXT("Player teleported to location: %s"), *SpawnLocation.ToString());
	}
}

void ATA_MainGameMode::AddPoint(EMapType MapType, FVector NewLocation)
{
	// 스폰 포인트가 추가되는지 확인하는 디버그 메시지
	UE_LOG(LogTemp, Log, TEXT("Added spawn point for MapType: %d at location: %s"), (int32)MapType, *NewLocation.ToString());
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
