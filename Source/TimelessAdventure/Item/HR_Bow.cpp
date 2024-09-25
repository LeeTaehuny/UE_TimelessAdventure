// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_Bow.h"


// Sets default values
AHR_Bow::AHR_Bow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHR_Bow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

