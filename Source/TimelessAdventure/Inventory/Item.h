// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class TIMELESSADVENTURE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void OnPickUp(class ATA_PlayerCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void OnDrop(class ATA_PlayerCharacter* Player);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* ItemMeshComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
