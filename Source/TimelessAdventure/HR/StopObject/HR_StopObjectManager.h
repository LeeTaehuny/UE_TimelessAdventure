// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HR_StopObjectManager.generated.h"

UCLASS()
class TIMELESSADVENTURE_API AHR_StopObjectManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_StopObjectManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// StopObject 관리
private:
	UPROPERTY(VisibleAnywhere, Category = "StopObjects")
	TArray<class AHR_StopObjectBase*> StopObjects;

// Stop Objects Material 변경 함수
public:
	void ChangeMaterialToSelectableAll();
	void ChangeMaterialToDefaultAll();
	
};
