// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TA_BossMonster.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_BossMonster : public ACharacter
{
	GENERATED_BODY()

public:
	ATA_BossMonster();

protected:
	virtual void BeginPlay() override;


};
