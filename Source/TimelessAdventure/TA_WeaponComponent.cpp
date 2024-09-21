// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_WeaponComponent.h"

// Sets default values
ATA_WeaponComponent::ATA_WeaponComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATA_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATA_WeaponComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATA_WeaponComponent::Attack()
{
}

void ATA_WeaponComponent::PlayAttackAnimation()
{
}

