// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TA_CustomWidget.h"
#include "TA_Inventory.generated.h"

UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	IT_Consum,	// 소비
	IT_Misc,	// 기타
};

UCLASS()
class TIMELESSADVENTURE_API UTA_Inventory : public UTA_CustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void InitInventory();
	void UpdateInvenSlot();

private:
	UFUNCTION()
	void ChangeInventoryType_C();

	UFUNCTION()
	void ChangeInventoryType_M();


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Frame;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_ChangeC;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_ChangeM;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Gold;

	UPROPERTY()
	TArray<TObjectPtr<class UTA_Slot>> Slots;

private:
	EInventoryType InventoryType;
};
