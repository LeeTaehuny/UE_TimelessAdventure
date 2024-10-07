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

// Getter Setter
public:
	FORCEINLINE bool GetbIsDetected() const {return bIsDetected;};
	FORCEINLINE void SetbIsDetected(bool b) { bIsDetected = b; };
	
// 멤버 변수 
protected:
	// Tick에 사용
	bool bIsStopped = false;
	FTimerHandle StopTimer;
	// overlap에 사용 
	bool bIsDetected = false;

	class UHR_StopAbilityComponent_T* StopAbilityComponent;

// Components
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* ObjectMesh;

// Material
private:
	UPROPERTY(VisibleAnywhere, Category = "Materials")
	class UMaterialInterface* SelectableMI;
	UPROPERTY(VisibleAnywhere, Category = "Materials")
	class UMaterialInterface* ClickableMI;
	UPROPERTY(VisibleAnywhere, Category = "Materials")
	class UMaterialInterface* StopMI;


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

	UFUNCTION()
	void OnBeginMouse(AActor* TouchedActor);
	UFUNCTION()
	void OnEndMouse(AActor* TouchedActor);
	UFUNCTION()
	void OnMouseClicked(AActor* TouchedActor, FKey ButtonPressed);

// Material 변경 함수
public:
	void ChangeMaterialToSelectable();
	void ChangeMaterialToClickable();
	void ChangeMaterialToDefault();
	void ChangeMaterialToStop();

	
};
