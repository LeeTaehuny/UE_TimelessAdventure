// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/TA_BossController.h"
#include "Monster/TA_AIKeys.h"
#include "Monster/TA_AIState.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ATA_BossController::ATA_BossController()
{
	// Black board Asset을 찾아 가져오기
	ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/LeeTaes/AI/BB_Boss.BB_Boss'"));
	if (BBAssetRef.Object)
	{
		BB_Asset = BBAssetRef.Object;
	}

	// Behavior Tree Asset을 찾아 가져오기
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/LeeTaes/AI/BT_Boss.BT_Boss'"));
	if (BTAssetRef.Object)
	{
		BT_Asset = BTAssetRef.Object;
	}
}

void ATA_BossController::RunAI()
{
	// Blackboard Component를 받아옵니다
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	// Blackboard 설정에 성공했다면?
	if (UseBlackboard(BB_Asset, BlackboardPtr))
	{
		// 초기 상태 저장
		Blackboard->SetValueAsEnum(BBKEY_STATE, static_cast<uint8>(EBossState::BS_Idle));

		// BehaviorTree를 동작시킵니다.
		RunBehaviorTree(BT_Asset);
	}
}

void ATA_BossController::StopAI()
{
	// BehaviorTree Component를 받아옵니다.
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BTComponent)
	{
		// BehaviorTree를 정지시킵니다.
		BTComponent->StopTree();
	}
}

void ATA_BossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AI 동작을 실행합니다.
	RunAI();
}

void ATA_BossController::OnUnPossess()
{
	Super::OnUnPossess();

	// AI 동작을 종료합니다.
	StopAI();
}
