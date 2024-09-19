// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_PlayerAnimInstance.h"
#include "Interface/AnimUpdateInterface.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTA_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ACharacter* PlayerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		Player = PlayerCharacter;
		MovementComponent = PlayerCharacter->GetCharacterMovement();
		CurrentWorldLocation = Player->GetActorLocation();
	}
}

void UTA_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MovementComponent)
	{
		// 현재 속도 저장
		Speed = MovementComponent->Velocity.Size2D();
		// 현재 가속도 저장 (X, Y)
		Acceleration = MovementComponent->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
		// 현재 가속도가 있는지 체크 (존재하면 true)
		bHasAccelation = !Acceleration.IsNearlyZero();
		
		// 현재 플레이어가 공중에 떠있는지 체크
		bIsFalling = MovementComponent->IsFalling();
	}

	if (Player)
	{
		// 현재 플레이어의 이동량 저장 (X, Y)
		Velocity = Player->GetVelocity() * FVector(1.0f, 1.0f, 0.0f);
		// 현재 이동량이 존재하는지 체크 (존재하면 true)
		bHasVelocity = !Velocity.IsNearlyZero();
		// 이전 프레임의 이동 거리 체크
		DistanceLastUpdate = (Player->GetActorLocation() - CurrentWorldLocation).Size2D();
		// 현재 위치 저장
		CurrentWorldLocation = Player->GetActorLocation();
		// 현재 스피드 저장
		DeltaSpeed = DistanceLastUpdate / DeltaSeconds;
	}
}
