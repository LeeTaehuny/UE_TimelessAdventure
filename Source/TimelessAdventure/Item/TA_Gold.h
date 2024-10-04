// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/TA_ItemBase.h"
#include "TA_Gold.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API ATA_Gold : public ATA_ItemBase
{
	GENERATED_BODY()
	
public:
	ATA_Gold();

protected:
	virtual void Interaction(ACharacter* Target) override;

private:
	UPROPERTY(EditAnywhere)
	int32 GlodAmount;
};
