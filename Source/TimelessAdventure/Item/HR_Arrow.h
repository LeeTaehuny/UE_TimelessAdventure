// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HR_WeaponBase.h"
#include "HR_Arrow.generated.h"

UCLASS()
class TIMELESSADVENTURE_API AHR_Arrow : public AHR_WeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_Arrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Components
protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* ProjectilMovementComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* StaticMeshComp;


//
public:
	void Fire(FVector Direction);
	
};
