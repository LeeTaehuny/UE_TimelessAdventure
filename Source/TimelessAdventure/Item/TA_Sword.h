﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/TA_WeaponBase.h"
#include "TA_Sword.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API ATA_Sword : public ATA_WeaponBase
{
	GENERATED_BODY()
	
public:
	ATA_Sword();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class USphereComponent> WeaponCollider;
	
};
