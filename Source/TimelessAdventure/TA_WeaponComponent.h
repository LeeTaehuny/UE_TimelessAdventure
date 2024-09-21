// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TA_WeaponComponent.generated.h"
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class TIMELESSADVENTURE_API ATA_WeaponComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATA_WeaponComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void PlayAttackAnimation();
};


