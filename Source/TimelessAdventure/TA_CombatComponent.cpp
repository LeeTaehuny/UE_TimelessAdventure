// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_CombatComponent.h"

UTA_CombatComponent::UTA_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Variable Initialize
	MaxStamina = 10.0f;
	CurrentStamina = 0.0f;
	MaxHp = 10.0f;
	CurrrentHp = 0.0f;
	bUseStamina = false;
	UseStaminaPercent = 0.001f;
}

void UTA_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UTA_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Stamina update
	UpdateStamina(bUseStamina, UseStaminaPercent);

	// Debug
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("CurHealth : %.1f"), CurrentStamina));
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, FString::Printf(TEXT("Rate      : %.1f"), CurrentStamina / MaxStamina));
}

float UTA_CombatComponent::GetStaminaPercent()
{
	if (MaxStamina == 0.0f) return MaxStamina;
	
	return CurrentStamina / MaxStamina;
}

void UTA_CombatComponent::SetUseStamina(bool Value)
{
	bUseStamina = Value;
}

void UTA_CombatComponent::Init()
{
	// Initialize value (Stamina, HP)
	CurrentStamina = MaxStamina;
	CurrrentHp = MaxHp;
}

void UTA_CombatComponent::UpdateStamina(bool Value, float Percent)
{
	// Not use stamina
	if (!Value && CurrentStamina < MaxStamina)
	{
		// Recovery as used percent
		CurrentStamina += (Percent * MaxStamina);

		// Set stamina less than Maxstamina
		if (CurrentStamina >= MaxStamina)
		{
			CurrentStamina = MaxStamina;
		}
	}
	// Use Stamina && Current Stamina > 0
	else if (Value && CurrentStamina > 0)
	{
		// decrease stamina by percent
		CurrentStamina -= (Percent * MaxStamina);

		// Stamina under zero
		if (CurrentStamina <= 0.0f)
		{
			// set Stamina zero
			CurrentStamina = 0.0f;
			// call function with deligate
			ZeroHealthDelegate.Broadcast();
			// set stamina increase
			bUseStamina = false;
		}
	}
}

