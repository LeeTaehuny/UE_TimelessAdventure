// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StaticObject.h"


// Sets default values
AHR_StaticObject::AHR_StaticObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHR_StaticObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_StaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHR_StaticObject::Move(float DeltaTime)
{
}

