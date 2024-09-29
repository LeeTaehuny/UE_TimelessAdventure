// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_Sword.h"


// Sets default values
AHR_Sword::AHR_Sword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon StaticMesh"));
	RootComponent = WeaponMesh;
	
}

// Called when the game starts or when spawned
void AHR_Sword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

