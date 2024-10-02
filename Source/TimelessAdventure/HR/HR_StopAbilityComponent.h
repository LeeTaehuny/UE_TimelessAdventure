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

	// click시 stop 모드 종료 + 모드 사용할 수 있는지 시간 체크

// input에 대한 처리, Stop Ability 사용
public:
	void StopAbilityBegin();
	void StopAbilityEnd();

// Mouse 이벤트 on off 함수
private:
	void ActivateMouseEvent();
	void DeactivateMouseEvent();

// 자주 사용하는 PlayerController 할당
private:
	class APlayerController* PlayerController;

// Collider Component
private:
	TObjectPtr<class USphereComponent> SphereCollider;
	
// Stop Object detect Radius
protected:
	UPROPERTY(EditAnywhere, Category="Detect Radius")
	float Radius = 500.f;

private:
	// 임시
	UPROPERTY(EditAnywhere, Category = "Stop Object Manager")
	class AHR_StopObjectManager* StopObjectManager;
	
};
