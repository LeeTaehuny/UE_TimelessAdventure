// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_Arrow.h"


// Sets default values
AHR_Arrow::AHR_Arrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHR_Arrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

