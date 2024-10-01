// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Monster/AnimNotify_ClearFocus.h"
#include "Monster/TA_BossController.h"

#include "GameFramework/Character.h"

void UAnimNotify_ClearFocus::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (ACharacter* Monster = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (ATA_BossController* MC = Cast<ATA_BossController>(Monster->GetController()))
		{
			MC->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}
