// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_TestObjectMoveForward.h"


// Sets default values
AHR_TestObjectMoveForward::AHR_TestObjectMoveForward()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHR_TestObjectMoveForward::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_TestObjectMoveForward::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHR_TestObjectMoveForward::Move(float DeltaTime)
{
	// 일단 등속 이동
	FVector newLocation = GetActorLocation() + (GetActorForwardVector() * DeltaTime * 100.f);
	SetActorLocation(newLocation);
	
}

