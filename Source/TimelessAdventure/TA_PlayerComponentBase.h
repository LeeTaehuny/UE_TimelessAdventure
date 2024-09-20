// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TA_PlayerComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMELESSADVENTURE_API UTA_PlayerComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTA_PlayerComponentBase();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Owner Player
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> OwnerPlayer;
		
};
