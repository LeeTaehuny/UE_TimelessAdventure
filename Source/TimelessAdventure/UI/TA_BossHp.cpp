// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_BossHp.h"

#include "Components/ProgressBar.h"

void UTA_BossHp::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTA_BossHp::SetPBPercentHealth(float Percent)
{
	PB_BossHp->SetPercent(Percent);
}