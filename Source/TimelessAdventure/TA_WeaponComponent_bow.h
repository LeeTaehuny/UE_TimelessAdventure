// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_WeaponComponent.h"
#include "TA_WeaponComponent_bow.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TIMELESSADVENTURE_API UTA_WeaponComponent_bow : public UTA_WeaponComponent
{
	GENERATED_BODY()
public:
	UTA_WeaponComponent_bow();

	// 활에 맞는 공격 함수
	virtual void Attack() override;
	
};
