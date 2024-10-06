// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HR_StopAbilityComponent_T.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMELESSADVENTURE_API UHR_StopAbilityComponent_T : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHR_StopAbilityComponent_T();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnRegister() override;

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

// Collider Scene Component
protected:
	UPROPERTY(VisibleAnywhere, Category = "Collider");
	TObjectPtr<class USphereComponent> DetectCollider;
	UPROPERTY(VisibleAnywhere, Category = "Collider");
	TObjectPtr<class UStaticMeshComponent> OverlayMesh;
	
// Stop Object Collider Radius
protected:
	UPROPERTY(EditAnywhere, Category="Detect Radius")
	float Radius = 500.f;
private:
	bool bIsTabClick = false;
	// 시간 게이지
	float MaxTimeEnergy = 50;
	float CurretTimeEnergy = 50;

	float Consumption = 10;

// 시간 게이지 계산
public:
	void SetTimeEnergy();

	// 시간 사용
	void UseTimeEnergy();

// Collision event
protected:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// 장막 visible

	
};
