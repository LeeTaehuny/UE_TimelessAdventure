// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "Data/TA_ItemData.h"
#include "Data/TA_MapType.h"
#include "TA_Stone.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_Stone : public AActor,
	public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ATA_Stone();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void Interaction(ACharacter* Target) override;

public:
	FORCEINLINE const TArray<FItemData>& GetShopItemData() { return ShopItemData; }
	FORCEINLINE const TArray<FName>& GetShopItemName() { return ShopItems; }

public:	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetVisibilityStoneMenu(bool Value);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USphereComponent> InteractionCollider;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class UStaticMeshComponent> ItemMeshComp;

	UPROPERTY(EditAnywhere, Category = "Item")
	FText InteractionText;

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<FName> ShopItems;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TArray<FItemData> ShopItemData; 

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UTA_StoneMenu> StoneWidgetClass;

	UPROPERTY()
	TObjectPtr<class UTA_StoneMenu> StoneWidget;

private:
	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = "true"))
	EMapType Type;

	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = "true"), meta = (MakeEditWidget = true))
	FVector TargetLocation;
};
