// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_JumpBack.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UBTT_JumpBack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_JumpBack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	float JumpBackDistance;
};
