// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Service/BTS_Detect.h"
#include "Monster/TA_AIKeys.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_Detect::UBTS_Detect()
{
	// 호출 간격 설정
	Interval = 1.0f;

	// 탐지 거리 설정
	DetectDistance = 1000.0f;
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 몬스터 Pawn 받아오기
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(ControllingPawn);

	// SphereTrace 진행
	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		ControllingPawn->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(DetectDistance),
		Params
	);

	// 충돌이 발생한 경우
	if (bHasHit)
	{
		// 순회하며 플레이어인지 체크
		for (const FOverlapResult& Result : OverlapResults)
		{
			// Character 타입으로 형변환
			ACharacter* Character = Cast<ACharacter>(Result.GetActor());
			// Character 유효성 및 플레이어인지 체크
			if (Character && Character->GetController()->IsPlayerController())
			{
				// 플레이어인 경우이므로 BlackBoard에 추가
				OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, Character);
			}
		}
	}
}