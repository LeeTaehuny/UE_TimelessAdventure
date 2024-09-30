// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "Data/TA_SlotType.h"
#include "TA_Slot.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_Slot : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	FORCEINLINE void SetNum(int32 Num) { SlotNum = Num; }
	void Init(ESlotType Type);
	void UpdateSlot();


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Quantity;

	ESlotType SlotType;

	UPROPERTY(EditAnywhere, Category = "Slot", meta = (AllowPrivateAccess = "true"))
	int32 SlotNum;

	UPROPERTY(EditAnywhere, Category = "Slot", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> NoneTexture;

	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<UTA_Slot> DragSlotClass;
};
