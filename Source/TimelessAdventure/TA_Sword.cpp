// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_Sword.h"

// Sets default values
ATA_Sword::ATA_Sword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the skeletal mesh component for the sword
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

}

// Called when the game starts or when spawned
void ATA_Sword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATA_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

