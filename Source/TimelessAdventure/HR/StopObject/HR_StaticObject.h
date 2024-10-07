// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HR_StopObjectBase.h"
#include "HR_StaticObject.generated.h"

UCLASS()
class TIMELESSADVENTURE_API AHR_StaticObject : public AHR_StopObjectBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_StaticObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Move(float DeltaTime) override;

};
