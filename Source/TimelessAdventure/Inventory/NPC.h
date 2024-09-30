// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/InteractableInterface.h"
#include "NPC.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ANPC : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, Category = "Interaction")
	TSubclassOf<class UUserWidget> InteractionWidgetClass;

protected :
	UPROPERTY(EditAnywhere, Category = "Interaction")
	class UWidgetComponent* InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<class UUserWidget> ItemShopWidgetClass;
	UPROPERTY()
	class UUserWidget* ItemShopWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FName> ItemOnSale;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void DisplayInteractionWidget() override;
	virtual void HideInteractionWidget() override;
	virtual void Interact() override;
};
