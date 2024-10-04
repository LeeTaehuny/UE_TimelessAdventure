// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/TA_PlayerComponentBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "TA_GrapRotateComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMELESSADVENTURE_API UTA_GrapRotateComponent : public UTA_PlayerComponentBase
{
	GENERATED_BODY()
public:
	UTA_GrapRotateComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunction);

	void updatewidget();
	void TraceValidTarget();
	void LineTraceFromCamera(FHitResult& OutHit, bool& bHit);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> WidgetClass;

	void PickupObject();
	void DropObject();
	UPROPERTY()
	class ATA_Movable* HeldObject;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI", meta = (AllowPrivateAccess = "true"))
	UUserWidget* CrosshairWidget;

private:
	FTimerHandle CrosshairTimerHandle;

	UPROPERTY(EditAnywhere)
	UCurveFloat* LerpCurve;
	FTimeline LerpTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimeLine", meta = (AllowPrivateAccess = "true"))
	float LerpTimelineLength;

	
	FVector StartLocation;
	FVector EndLocation;
	FVector LerpStartLocation;
	FVector LerpEndLocation;

	float StartingDistance;
	FVector StartingScale;
	
	FVector SetObjectBounds();
	

public:
	void StartLerping(FVector NewStartLocation);
	void LerpProgress(float Value);
};
