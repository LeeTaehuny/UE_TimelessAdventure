// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TA_EquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_EquipmentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Bow;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Fist;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Sword;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Torch;


	int32 CurrentHoveredWeapon = 0;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBowHovered();

	UFUNCTION()
	void OnFistHovered();
	
	UFUNCTION()
	void OnSwordHovered();

	UFUNCTION()
	void OnTorchHovered();
	
	
	
	
	
};
