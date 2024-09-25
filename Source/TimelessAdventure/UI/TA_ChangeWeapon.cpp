// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_ChangeWeapon.h"
#include "Interface/CombatComponentInterface.h"
#include "Component/TA_CombatComponent.h"

#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"

void UTA_ChangeWeapon::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Fist->OnHovered.AddDynamic(this, &UTA_ChangeWeapon::OnHoverFist);
	BTN_Sword->OnHovered.AddDynamic(this, &UTA_ChangeWeapon::OnHoverSword);
	BTN_Bow->OnHovered.AddDynamic(this, &UTA_ChangeWeapon::OnHoverBow);
	BTN_Torch->OnHovered.AddDynamic(this, &UTA_ChangeWeapon::OnHoverTorch);
	BTN_Cancel->OnHovered.AddDynamic(this, &UTA_ChangeWeapon::OnHoverCancel);
}

void UTA_ChangeWeapon::OpenWidget()
{
	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(OL_Main);
	if (slot)
	{
		slot->SetPosition(MousePos);
	}

	PlayAnimation(ChangeWidgetOpen);
}

void UTA_ChangeWeapon::OnHoverFist()
{
	if (OwnerActor)
	{
		ICombatComponentInterface* CombatComponentInterface = Cast<ICombatComponentInterface>(OwnerActor);
		if (CombatComponentInterface)
		{
			CombatComponentInterface->GetCombatComponent()->SetChangeWeaponState(EEquippedState::ES_Idle);
		}
	}
}

void UTA_ChangeWeapon::OnHoverSword()
{
	if (OwnerActor)
	{
		ICombatComponentInterface* CombatComponentInterface = Cast<ICombatComponentInterface>(OwnerActor);
		if (CombatComponentInterface)
		{
			CombatComponentInterface->GetCombatComponent()->SetChangeWeaponState(EEquippedState::ES_Sword);
		}
	}
}

void UTA_ChangeWeapon::OnHoverBow()
{
	if (OwnerActor)
	{
		ICombatComponentInterface* CombatComponentInterface = Cast<ICombatComponentInterface>(OwnerActor);
		if (CombatComponentInterface)
		{
			CombatComponentInterface->GetCombatComponent()->SetChangeWeaponState(EEquippedState::ES_Bow);
		}
	}
}

void UTA_ChangeWeapon::OnHoverTorch()
{
	if (OwnerActor)
	{
		ICombatComponentInterface* CombatComponentInterface = Cast<ICombatComponentInterface>(OwnerActor);
		if (CombatComponentInterface)
		{
			CombatComponentInterface->GetCombatComponent()->SetChangeWeaponState(EEquippedState::ES_Torch);
		}
	}
}

void UTA_ChangeWeapon::OnHoverCancel()
{
	if (OwnerActor)
	{
		ICombatComponentInterface* CombatComponentInterface = Cast<ICombatComponentInterface>(OwnerActor);
		if (CombatComponentInterface)
		{
			CombatComponentInterface->GetCombatComponent()->SetChangeWeaponState(EEquippedState::ES_Cancel);
		}
	}
}
