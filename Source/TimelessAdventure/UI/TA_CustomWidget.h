// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TA_CustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_CustomWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetOwnerPlayer(AActor* Owner) { OwnerActor = Owner; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AActor> OwnerActor;
	
};
