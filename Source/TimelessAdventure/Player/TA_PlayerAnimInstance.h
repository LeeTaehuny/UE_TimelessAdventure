// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TA_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TIMELESSADVENTURE_API UTA_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	// Initialize
	virtual void NativeInitializeAnimation() override;

	// Update
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACharacter> Player;

public:
	// Bow Anim을 위한 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHasBow;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAimingWithBow;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DistanceMaching", meta = (AllowPrivateAccess = "true"))
	FVector Acceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DistanceMaching", meta = (AllowPrivateAccess = "true"))
	bool bHasAccelation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DistanceMaching", meta = (AllowPrivateAccess = "true"))
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DistanceMaching", meta = (AllowPrivateAccess = "true"))
	bool bHasVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DistanceMaching", meta = (AllowPrivateAccess = "true"))
	float DistanceLastUpdate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DistanceMaching", meta = (AllowPrivateAccess = "true"))
	FVector CurrentWorldLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DistanceMaching", meta = (AllowPrivateAccess = "true"))
	float DeltaSpeed;
};
