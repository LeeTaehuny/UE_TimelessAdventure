// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HR_StopObjectBase.generated.h"

// 이벤트 함수 선언 정의 & 바인드 

UCLASS(Abstract)
class TIMELESSADVENTURE_API AHR_StopObjectBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_StopObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 멤버 변수 
private:
	bool bIsStopped = false;
	FTimerHandle StopTimer;

// Components
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* ObjectMesh;

// Material
public:
	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterial* SelectableMaterial;
	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterial* ClickableMaterial;
	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterial* DefaultMaterial;
	
// 순수 가상 함수
protected:
	virtual void Move(float DeltaTime) PURE_VIRTUAL(AHR_StopableObjectBase::Move, );

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

// Material 변경 함수
public:
	void ChangeMaterialToSelectable();
	void ChangeMaterialToClickable();
	void ChangeMaterialToDefault();
	
	
	
};
