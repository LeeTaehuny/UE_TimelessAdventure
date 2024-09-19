// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingNotify.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
void URollingNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// Ensure the mesh component and owner exist
	if (MeshComp && MeshComp->GetOwner())
	{
		ATP_ThirdPersonCharacter* Character = Cast<ATP_ThirdPersonCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			//Character->bIsRolling = true;
			//Character->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		}
		UE_LOG(LogTemp, Warning, TEXT("RollingNotify Begin triggered for %f seconds"), TotalDuration);
		
	}
}

void URollingNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// Ensure the mesh component and owner exist
	if (MeshComp && MeshComp->GetOwner())
	{
		ATP_ThirdPersonCharacter* Character = Cast<ATP_ThirdPersonCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->bIsRolling = false;
			Character->GetCharacterMovement()->MaxWalkSpeed = Character->WalkSpeed;
		}
		UE_LOG(LogTemp, Warning, TEXT("RollingNotify End triggered"));
	}
}