// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TA_PlayerComponentBase.h"
#include "GameFramework/Actor.h"
#include "TA_WeaponComponent.generated.h"
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class TIMELESSADVENTURE_API UTA_WeaponComponent : public UTA_PlayerComponentBase

{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UTA_WeaponComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Attack();
	virtual void StartAttackSequence(UAnimInstance* AnimInstance, ACharacter* OwnerCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void PlayAttackAnimation();
	virtual void CancelAttackAndMove();
protected:
	// weapon Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

	// weapon distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;

	// weapon amination
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* AttackAnimation;

	// weapon status : Is attacking?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsAttacking;

	// play anim
	void PlayAnimation(UAnimMontage* Animation);

};


