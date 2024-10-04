// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Task/BTT_ChangeState.h"
#include "Interface/MonsterInterface.h"
#include "Monster/TA_AIKeys.h"

#include "Monster/TA_BossController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ChangeState::UBTT_ChangeState()
{

}

EBTNodeResult::Type UBTT_ChangeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			// 상태 변경
			MonsterInterface->ChangeState(NewState);
			// 블랙보드 키 값 변경
			AIController->GetBlackboardComponent()->SetValueAsEnum(BBKEY_STATE, static_cast<uint8>(NewState));
			// 성공 반환
			return EBTNodeResult::Succeeded;
		}
	}

	// 실패 반환
	return EBTNodeResult::Failed;
}
