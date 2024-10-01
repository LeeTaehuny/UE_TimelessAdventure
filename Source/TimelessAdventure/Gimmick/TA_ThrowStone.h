// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TA_ThrowStone.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_ThrowStone : public AActor
{
	GENERATED_BODY()
	
public:
	ATA_ThrowStone();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void Fire(AActor* Target, FVector Direction);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Setting")
	float InitSpeed;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float HomingAccelerationMagnitude;

};
