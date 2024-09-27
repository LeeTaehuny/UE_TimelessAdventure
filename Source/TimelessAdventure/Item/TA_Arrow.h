// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TA_Arrow.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_Arrow : public AActor
{
	GENERATED_BODY()
	
public:
	ATA_Arrow();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void FireArrow(FVector Pos);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> ShaftMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> IronMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

};
