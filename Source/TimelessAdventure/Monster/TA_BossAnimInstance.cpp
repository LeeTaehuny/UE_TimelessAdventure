// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/TA_BossAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTA_BossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ACharacter* MonsterCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (MonsterCharacter)
	{
		Monster = MonsterCharacter;
		MovementComponent = MonsterCharacter->GetCharacterMovement();
	}
}

void UTA_BossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MovementComponent)
	{
		// 현재 속도 저장
		Speed = MovementComponent->Velocity.Size2D();
		// 현재 몬스터가 공중에 떠있는지 체크
		bIsFalling = MovementComponent->IsFalling();
	}

	if (Monster)
	{
		// 현재 플레이어의 이동량 저장 (X, Y)
		Velocity = Monster->GetVelocity() * FVector(1.0f, 1.0f, 0.0f);

		// 방향 저장
		Direction = CalculateDirection(Velocity, Monster->GetActorRotation());
	}
}
