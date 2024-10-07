// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "TA_ItemBase.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_ItemBase : public AActor,
	public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ATA_ItemBase();

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

protected:
	virtual void Interaction(ACharacter* Target) override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckItem();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USphereComponent> InteractionCollider;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class UStaticMeshComponent> ItemMeshComp;

	UPROPERTY(EditAnywhere, Category = "Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, Category = "Item")
	FText InteractionText;
};
