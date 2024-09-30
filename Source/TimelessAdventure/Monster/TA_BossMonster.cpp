// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/TA_BossMonster.h"

// Sets default values
ATA_BossMonster::ATA_BossMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATA_BossMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATA_BossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATA_BossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

