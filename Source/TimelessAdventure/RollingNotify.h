// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RollingNotify.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API URollingNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public :
	// Override the NotifyBegin function
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	// Override the NotifyEnd function
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
