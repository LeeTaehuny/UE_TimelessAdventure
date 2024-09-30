// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HR_StopableObjectBase.generated.h"

// 이벤트 함수 선언 정의 & 바인드 

UCLASS(Abstract)
class TIMELESSADVENTURE_API AHR_StopableObjectBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_StopableObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//
private:
	bool isStopped = false;

// 순수 가상 함수
protected:
	virtual void Move() PURE_VIRTUAL(AHR_StopableObjectBase::Move, );

// 기본 Base 함수
protected:
	// Mouse over begin, over end, click
	// Mouse over begin > Actor의 Material Change(ClickableMaterial)
	// Mouse over end > Actor의 Material Change(SelectableMaterial)
	// Mouse Click > Material change?, Interact() 수행, Stop 능력 사용 종료 
		// Interact() -> isStopped 변수 세팅, Timer 설정
	void OnBeginMouse(AActor* TouchedActor);
	void OnEndMouse(AActor* TouchedActor);
	void OnMouseClicked(AActor* TouchedActor, FKey ButtonPressed);
	
	
	
	
	
};
