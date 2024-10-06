// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TA_Movable.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_Movable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATA_Movable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UStaticMeshComponent* GetMeshComponent();
	//UArrowComponent* GetArrowComponent();
	
	bool IsLocked;
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SelfDrop();
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	bool SelfPickup(USceneComponent* ParentComp);

	// Arrow Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* Arrow;
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MovableMesh;


	bool IsHeld;
};
