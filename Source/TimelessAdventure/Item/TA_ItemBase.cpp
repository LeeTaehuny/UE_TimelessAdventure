// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_ItemBase.h"

// Sets default values
ATA_ItemBase::ATA_ItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATA_ItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATA_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

