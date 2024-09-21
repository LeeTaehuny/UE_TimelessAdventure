// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TA_ComboAttackData.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_ComboAttackData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 콤보에 사용될 몽타주
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> ComboMontage;

	// 최대 콤보 수
	UPROPERTY(EditAnywhere, Category = "ComboData")
	int32 MaxCount;

	// 콤보를 이어가기 위한 최대 체크 시간
	UPROPERTY(EditAnywhere, Category = "ComboData")
	TArray<float> ComboFrame;

	// 콤보 몽타주 섹션 이름
	UPROPERTY(EditAnywhere, Category = "ComboData")
	FString SectionBaseName;

	// 애니메이션 재생 속도
	UPROPERTY(EditAnywhere, Category = "ComboData")
	float FrameRate;
};
