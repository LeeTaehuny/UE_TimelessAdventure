// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HR_ItemBase.generated.h"

UCLASS()
class TIMELESSADVENTURE_API AHR_ItemBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_ItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
