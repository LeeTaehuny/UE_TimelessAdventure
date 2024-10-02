// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Task/BTT_JumpBack.h"
#include "Interface/MonsterInterface.h"

#include "Monster/TA_BossController.h"

UBTT_JumpBack::UBTT_JumpBack()
{
	JumpBackDistance = 700.0f;
}

EBTNodeResult::Type UBTT_JumpBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI Controller 가져오기
	ATA_BossController* AIController = Cast<ATA_BossController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		// Monster Interface 가져오기
		IMonsterInterface* MonsterInterface = Cast<IMonsterInterface>(AIController->GetPawn());
		if (MonsterInterface)
		{
			// 공격 종료 델리게이트 생성
			FOnJumpBackEndDelegate OnJumpBackEndDelegate;
			OnJumpBackEndDelegate.BindLambda(
				[&]()
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			);

			MonsterInterface->SetAIJumpDelegate(OnJumpBackEndDelegate);

			// 점프 Back
			MonsterInterface->JumpBack(JumpBackDistance);

			// 진행 중 반환
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_JumpBack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}
