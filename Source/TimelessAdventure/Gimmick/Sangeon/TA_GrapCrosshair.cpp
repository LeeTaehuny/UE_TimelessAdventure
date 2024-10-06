// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_GrapCrosshair.h"

void UTA_GrapCrosshair::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTA_GrapCrosshair::UnHover()
{
	if(IMG_Crosshair)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnHover"));

		IMG_Crosshair->SetRenderOpacity(0.4f);
	}
	IsHover = false;
}

void UTA_GrapCrosshair::Hover()
{
	if(IMG_Crosshair)
	{
		IMG_Crosshair->SetRenderOpacity(1.0f);
	}
	IsHover = true;
}
