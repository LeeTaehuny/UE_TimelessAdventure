// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_StairActor.h"
#include "GameFramework/Actor.h"
#include "TA_ButtonActor.generated.h"

UCLASS()
class TIMELESSADVENTURE_API ATA_ButtonActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATA_ButtonActor();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void MoveButton(FVector TargetPosition);
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	ATA_StairActor* FirstStair;
private:
	FVector InitialPosition;
	FVector PressedPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ButtonMesh;


	
	FTimerHandle ButtonMoveTimer;

};
