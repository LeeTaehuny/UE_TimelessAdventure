// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/TA_MapType.h"
#include "TA_MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API ATA_MainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATA_MainGameMode();

protected:
	virtual void BeginPlay() override;

public:
	void StopBGM();
	void RespawnPlayer();
	void TeleportPlayer(EMapType MapType);
	void AddPoint(EMapType MapType, FVector NewLocation);
	void SetRespawnLocation(FVector NewLocation);
	void SetBoss(AActor* BossActor);
	void DeathBoss();
	FVector GetPoint(EMapType MapType);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTeleportRuins() { bCanTeleportRuins = true; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTeleportCave() { bCanTeleportCave = true; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetBossSpawnable() { return !IsValid(Boss); }

	FORCEINLINE bool GetTeleportPrairie() { return bCanTeleportPrairie; }
	FORCEINLINE bool GetTeleportRuins() { return bCanTeleportRuins; }
	FORCEINLINE bool GetTeleportCave() { return bCanTeleportCave; }

private:
	UPROPERTY(VisibleAnywhere)
	TMap<EMapType, FVector> SpawnPoints;

	UPROPERTY(VisibleAnywhere)
	FVector RespawnLocation;

	UPROPERTY(VisibleAnywhere)
	bool bCanTeleportPrairie;

	UPROPERTY(VisibleAnywhere)
	bool bCanTeleportRuins;

	UPROPERTY(VisibleAnywhere)
	bool bCanTeleportCave;

	UPROPERTY()
	TObjectPtr<AActor> Boss;

private:
	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> BGM_Basic;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> BGM_Boss;
};
