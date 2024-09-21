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
	// �޺��� ���� ��Ÿ��
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> ComboMontage;

	// �ִ� �޺� ��
	UPROPERTY(EditAnywhere, Category = "ComboData")
	int32 MaxCount;

	// �޺��� �̾�� ���� �ִ� üũ �ð�
	UPROPERTY(EditAnywhere, Category = "ComboData")
	TArray<float> ComboFrame;

	// �޺� ��Ÿ�� ���� �̸�
	UPROPERTY(EditAnywhere, Category = "ComboData")
	FString SectionBaseName;

	// �ִϸ��̼� ��� �ӵ�
	UPROPERTY(EditAnywhere, Category = "ComboData")
	float FrameRate;
};
