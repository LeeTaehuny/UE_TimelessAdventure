// Fill out your copyright notice in the Description page of Project Settings.
#include "Gimmick/Sangeon/TA_GrapRotateComponent.h"
#include "Blueprint/UserWidget.h"

void UTA_GrapRotateComponent::CreateWidget() const
{
	if(!GetWorld())
	{
		return;
	}
	if(WidgetClass)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

		if(WidgetInstance)
		{
			WidgetInstance->AddToViewport();
		}
	}
}
