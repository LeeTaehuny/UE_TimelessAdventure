// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Decorator/BTD_IsHalfHealth.h"
#include "Interface/MonsterInterface.h"
#include "Monster/TA_BossController.h"

bool UBTD_IsHalfHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AI Controller 가져오기
	ATA_BossController* AIController = Cast<ATA_BossController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		// Monster Interface 가져오기
		IMonsterInterface* MonsterInterface = Cast<IMonsterInterface>(AIController->GetPawn());
		if (MonsterInterface)
		{
			// 체력 체크
			if (MonsterInterface->GetHealthPercent() >= 0.5f)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}
