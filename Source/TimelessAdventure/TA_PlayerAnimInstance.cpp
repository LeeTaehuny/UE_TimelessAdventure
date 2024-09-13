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
		PrevLocation = Player->GetActorLocation();
	}
}

void UTA_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MovementComponent)
	{
		Speed = MovementComponent->Velocity.Size2D();
		Acceleration = MovementComponent->GetCurrentAcceleration();
	}

	if (Player)
	{
		DistanceLastUpdate = (PrevLocation - Player->GetActorLocation()).Size2D();
		DeltaSpeed = DistanceLastUpdate / DeltaSeconds;

		PrevLocation = Player->GetActorLocation();
	}
}
