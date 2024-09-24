// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_HUD.h"

#include "Components/Image.h"

void UTA_HUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTA_HUD::SetAimVisibility(bool Value)
{
	IMG_Aim->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}