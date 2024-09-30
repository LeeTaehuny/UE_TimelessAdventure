// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TA_PlayerComponentBase.h"
#include "TA_InteractionComponent.generated.h"

/**
 * 
 */


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMELESSADVENTURE_API UTA_InteractionComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()
public:
	UTA_InteractionComponent();


	virtual void BeginPlay() override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InteractionPositive();
	void PerformInteractionTrace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int64 CurrentMoney = 9999;

	UFUNCTION(BlueprintCallable)
	void UpdateMoney(int64 inputval);

private:
	AActor* CachedInteractableActor;
};


