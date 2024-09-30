// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TA_Sword.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_Sword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATA_Sword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
