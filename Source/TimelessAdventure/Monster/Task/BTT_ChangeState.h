// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Monster/TA_AIState.h"
#include "BTT_ChangeState.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UBTT_ChangeState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_ChangeState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	EBossState NewState;
};
