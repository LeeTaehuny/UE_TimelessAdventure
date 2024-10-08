// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "TA_EndingActor.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_EndingActor : public AActor,
	public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ATA_EndingActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void Interaction(ACharacter* Target) override;

public:
	void OpenDoor();
	void VisibleFirstCrystal();
	void VisibleSecondCrystal();
	void VisibleThirdCrystal();


	UFUNCTION(BlueprintImplementableEvent)
	void StartEnding();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USphereComponent> InteractionCollider;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class UStaticMeshComponent> Crystal1;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class UStaticMeshComponent> Crystal2;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class UStaticMeshComponent> Crystal3;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class UStaticMeshComponent> Door;

private:
	bool bIsOpen;
	FVector DoorWorldLocation;

	bool bFirstVisible;
	bool bSecondVisible;
	bool bThirdVisible;
};
