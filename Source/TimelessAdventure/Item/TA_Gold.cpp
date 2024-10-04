// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_Gold.h"
#include "Interface/InventoryInterface.h"
#include "Component/TA_InventoryComponent.h"
#include "Interface/CombatComponentInterface.h"
#include "Component/TA_CombatComponent.h"

#include "GameFramework/Character.h"

ATA_Gold::ATA_Gold()
{
	GlodAmount = 10.0f;
}

void ATA_Gold::Interaction(ACharacter* Target)
{
	if (!Target && !Target->GetMesh()->GetAnimInstance()) return;

	// Combat 컴포넌트가 존재하는 경우
	if (ICombatComponentInterface* CombatInterface = Cast<ICombatComponentInterface>(Target))
	{
		Target->GetMesh()->GetAnimInstance()->Montage_Play(CombatInterface->GetCombatComponent()->GetPickupMontage());
	}

	// Inventory 컴포넌트가 존재하는 경우
	if (IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(Target))
	{
		// 골드 추가
		InventoryInterface->GetInventory()->AddGold(GlodAmount);
		// 현재 아이템 제거
		Destroy();
	}
}
