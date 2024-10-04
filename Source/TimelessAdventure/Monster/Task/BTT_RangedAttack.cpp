// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Task/BTT_RangedAttack.h"
#include "Interface/MonsterInterface.h"

#include "Monster/TA_BossController.h"

UBTT_RangedAttack::UBTT_RangedAttack()
{
}

EBTNodeResult::Type UBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			// 공격
			MonsterInterface->RangedAttack();

			// 공격 종료 델리게이트 생성
			FOnAttackEndDelegate OnAttackEndDelegate;
			OnAttackEndDelegate.BindLambda(
				[&]()
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			);

			MonsterInterface->SetAIAttackDelegate(OnAttackEndDelegate);
			
			// 진행 중 반환
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_RangedAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}
