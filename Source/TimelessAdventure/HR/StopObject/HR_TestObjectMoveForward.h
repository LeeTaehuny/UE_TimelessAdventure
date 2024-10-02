// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HR_StopObjectBase.h"
#include "HR_TestObjectMoveForward.generated.h"

UCLASS()
class TIMELESSADVENTURE_API AHR_TestObjectMoveForward : public AHR_StopObjectBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_TestObjectMoveForward();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Move
protected:
	virtual void Move(float DeltaTime) override;	

// setting 변수
public:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float MoveSpeed;

};
