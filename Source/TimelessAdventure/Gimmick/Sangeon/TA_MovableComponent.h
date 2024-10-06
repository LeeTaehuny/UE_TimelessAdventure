// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TA_MovableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class TIMELESSADVENTURE_API UTA_MovableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTA_MovableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UStaticMeshComponent* GetMeshComponent();
	bool IsLocked;
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SelfDrop();
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	bool SelfPickup(USceneComponent* ParentComp);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MovableMesh;
private:



	bool IsHeld;
};
