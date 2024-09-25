// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HR_WeaponBase.h"
#include "HR_Bow.generated.h"

// Bow 상태
UENUM(BlueprintType)
enum class EBowState : uint8
{
	BS_Idle,
	BS_Aim,
	BS_Draw
};

UCLASS()
class TIMELESSADVENTURE_API AHR_Bow : public AHR_WeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHR_Bow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Components
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USkeletalMeshComponent* SkeletalMeshComp;

// Bow 상태
private:
	UPROPERTY(VisibleAnywhere, Category = "Bow")
	EBowState BowState;


// Bow state
public:
	UFUNCTION(BlueprintCallable)
	void ChangeBowState(EBowState newBowState);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EBowState GetBowState() const { return BowState; }

public:
	void SpawnArrow(USkeletalMeshComponent* Mesh);

	
public:
	UPROPERTY(EditAnywhere, Category = "Arrow")
	TSubclassOf<class AHR_Arrow> ArrowClass;
	
};
