// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_WeaponBase.h"


// Sets default values
AHR_WeaponBase::AHR_WeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHR_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

