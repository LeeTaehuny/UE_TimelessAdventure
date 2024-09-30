// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/TA_BossController.h"
#include "Monster/TA_AIKeys.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ATA_BossController::ATA_BossController()
{
	ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/LeeTaes/AI/BB_Boss.BB_Boss'"));
	if (BBAssetRef.Object)
	{
		BB_Asset = BBAssetRef.Object;
	}

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
		// 시작 지점 저장
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

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
