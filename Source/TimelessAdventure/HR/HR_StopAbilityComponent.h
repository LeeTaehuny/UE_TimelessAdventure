// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/TA_PlayerComponentBase.h"
#include "HR_StopAbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMELESSADVENTURE_API UHR_StopAbilityComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHR_StopAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
