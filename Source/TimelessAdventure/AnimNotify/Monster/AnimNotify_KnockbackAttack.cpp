// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Monster/AnimNotify_KnockbackAttack.h"
#include "Interface/MonsterInterface.h"

void UAnimNotify_KnockbackAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IMonsterInterface* MonsterInterface = Cast<IMonsterInterface>(MeshComp->GetOwner()))
	{
		MonsterInterface->KnockbackAttackCheck();
	}
}
