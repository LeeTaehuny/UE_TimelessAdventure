// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HR_StopObjectBase.h"
#include "HR_TrickWall.generated.h"

UCLASS()
class TIMELESSADVENTURE_API AHR_TrickWall : public AHR_StopObjectBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_TrickWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Move(float DeltaTime) override;
	virtual void Stop() override;

protected:
	// 방향 전환 체크 함수
	void CheckMoveDir();
	
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult & SweepResult);
	
// member
private:
	bool bMovingForward = true;

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsTrigger = false;

protected:
	UPROPERTY(EditAnywhere, Category = "Move point")
	float StartLocationY;
	UPROPERTY(EditAnywhere, Category = "Move point")
	float EndLocationY;
};
