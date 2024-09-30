// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StopableObjectBase.h"


// Sets default values
AHR_StopableObjectBase::AHR_StopableObjectBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnBeginCursorOver.AddDynamic(this, &AHR_StopableObjectBase::OnBeginMouse);
	OnEndCursorOver.AddDynamic(this, &AHR_StopableObjectBase::OnEndMouse);
	OnClicked.AddDynamic(this, &AHR_StopableObjectBase::OnMouseClicked);
}

// Called when the game starts or when spawned
void AHR_StopableObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_StopableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 바인딩 함수 
void AHR_StopableObjectBase::OnBeginMouse(AActor* TouchedActor)
{
}

void AHR_StopableObjectBase::OnEndMouse(AActor* TouchedActor)
{
}

void AHR_StopableObjectBase::OnMouseClicked(AActor* TouchedActor, FKey ButtonPressed)
{
}





