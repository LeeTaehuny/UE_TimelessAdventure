// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Task/BTT_FocusConvertTarget.h"
#include "Monster/TA_BossController.h"
#include "Monster/TA_AIKeys.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_FocusConvertTarget::UBTT_FocusConvertTarget()
{
}

EBTNodeResult::Type UBTT_FocusConvertTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI Controller 가져오기
	ATA_BossController* AIController = Cast<ATA_BossController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		if (bIsFocus)
		{
			// 플레이어 액터 가져오기
			AActor* PlayerActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));

			// 포커스 설정
			AIController->SetFocus(PlayerActor);
		}
		else
		{
			// 포커스 해제
			EAIFocusPriority::Type Priority = EAIFocusPriority::Default;
			UE_LOG(LogTemp, Warning, TEXT("Focus False"));
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
		}

		// 성공 처리
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_FocusConvertTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}
