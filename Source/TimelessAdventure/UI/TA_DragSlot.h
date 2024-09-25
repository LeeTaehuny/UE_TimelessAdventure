// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Data/TA_SlotType.h"
#include "TA_DragSlot.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_DragSlot : public UDragDropOperation
{
	GENERATED_BODY()
	
// Drag 정보 저장용 변수
public:
	UPROPERTY(VisibleAnywhere)
	int PrevSlotIndex;

	UPROPERTY(VisibleAnywhere)
	ESlotType SlotType;
};
