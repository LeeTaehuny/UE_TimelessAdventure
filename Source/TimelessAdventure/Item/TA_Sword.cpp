// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_Sword.h"

#include "Components/SphereComponent.h"

ATA_Sword::ATA_Sword()
{
	WeaponType = EWeaponType::WT_Sword;

	WeaponSocketName = TEXT("SwordSocket");

	WeaponCollider = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollider"));
	WeaponCollider->SetupAttachment(RootComponent);
}
