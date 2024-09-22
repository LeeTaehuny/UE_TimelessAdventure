// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Player/AnimNotify_ReleaseString.h"
#include "Interface/CombatComponentInterface.h"
#include "Component/TA_CombatComponent.h"
#include "Item/TA_Bow.h"

void UAnimNotify_ReleaseString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (ICombatComponentInterface* CombatPlayer = Cast<ICombatComponentInterface>(MeshComp->GetOwner()))
	{
		// 활로 형변환
		if (ATA_Bow* BowWeapon = Cast<ATA_Bow>(CombatPlayer->GetCombatComponent()->GetEquippedWeapon()))
		{
			BowWeapon->SetIsHold(false);
		}
	}
}
