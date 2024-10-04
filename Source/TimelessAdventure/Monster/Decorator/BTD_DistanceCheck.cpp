// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Decorator/BTD_DistanceCheck.h"
#include "Monster/TA_BossController.h"
#include "Monster/TA_AIKeys.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTD_DistanceCheck::UBTD_DistanceCheck()
{
	CheckDistance = 1000.0f;
}

bool UBTD_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AI Controller 가져오기 
	ATA_BossController* AIController = Cast<ATA_BossController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		// 플레이어 찾기
		AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
		if (Target)
		{
			// 플레이어와 자신 사이의 거리 체크
			float Distance = (Target->GetActorLocation() - AIController->GetPawn()->GetActorLocation()).Length();

			if (Distance >= CheckDistance)
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
