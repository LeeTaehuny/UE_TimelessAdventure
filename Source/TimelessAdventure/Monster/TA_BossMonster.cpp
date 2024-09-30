// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/TA_BossMonster.h"
#include "Monster/TA_BossController.h"


ATA_BossMonster::ATA_BossMonster()
{
 	PrimaryActorTick.bCanEverTick = false;

	// AI Controller Setting
	AIControllerClass = ATA_BossController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATA_BossMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

