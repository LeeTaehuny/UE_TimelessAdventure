// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Player/AnimNotify_PlayerDeath.h"
#include "Interface/CombatComponentInterface.h"
#include "Component/TA_CombatComponent.h"

void UAnimNotify_PlayerDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (ICombatComponentInterface* CombatPlayer = Cast<ICombatComponentInterface>(MeshComp->GetOwner()))
	{
		CombatPlayer->GetCombatComponent()->Respawn();
	}
}
