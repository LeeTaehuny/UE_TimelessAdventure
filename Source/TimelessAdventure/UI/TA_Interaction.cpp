// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_Interaction.h"

#include "Components/TextBlock.h"

void UTA_Interaction::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTA_Interaction::UpdateHelpText(FText NewText)
{
	TXT_HelpText->SetText(NewText);
}