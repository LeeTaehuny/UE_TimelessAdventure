// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TA_BossController.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API ATA_BossController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATA_BossController();

	// AI Start
	void RunAI();
	// AI Stop
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	// BlackBoard, BehaviorTree
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBlackboardData> BB_Asset;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBehaviorTree> BT_Asset;
};
