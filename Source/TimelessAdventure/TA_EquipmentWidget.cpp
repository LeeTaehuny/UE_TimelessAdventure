// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_EquipmentWidget.h"

#include "Components/Button.h"

void UTA_EquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(Btn_Bow)
	{
		Btn_Bow->OnHovered.AddDynamic(this, &UTA_EquipmentWidget::OnBowHovered);
	}
	if(Btn_Sword)
	{
		Btn_Sword->OnHovered.AddDynamic(this, &UTA_EquipmentWidget::OnSwordHovered);
	}
	if(Btn_Fist)
	{
		Btn_Fist->OnHovered.AddDynamic(this, &UTA_EquipmentWidget::OnFistHovered);
	}
	if(Btn_Torch)
	{
		Btn_Torch->OnHovered.AddDynamic(this, &UTA_EquipmentWidget::OnTorchHovered);
	}
}

void UTA_EquipmentWidget::OnBowHovered()
{
	CurrentHoveredWeapon = 2;
}

void UTA_EquipmentWidget::OnFistHovered()
{
	CurrentHoveredWeapon = 4;
}

void UTA_EquipmentWidget::OnSwordHovered()
{
	CurrentHoveredWeapon = 1;
}

void UTA_EquipmentWidget::OnTorchHovered()
{
	CurrentHoveredWeapon = 3;
}
