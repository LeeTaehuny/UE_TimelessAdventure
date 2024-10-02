// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_DistanceCheck.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UBTD_DistanceCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_DistanceCheck();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere)
	float CheckDistance;
};
