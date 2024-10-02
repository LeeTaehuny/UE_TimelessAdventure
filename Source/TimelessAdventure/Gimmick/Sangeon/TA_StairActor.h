// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TA_StairActor.generated.h"
UENUM(BlueprintType)
enum class EstairMovementType : uint8
{
	None UMETA(DisplayName = "None"),
	Vertical UMETA(DisplayName = "Vertical"),
	Horizontal UMETA(DisplayName = "Horizontal")
};
UCLASS()
class TIMELESSADVENTURE_API ATA_StairActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATA_StairActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//플레이어가 밟았을 때
	UFUNCTION()
	void OnPlayerStep(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//다음 징검다리 show
	void ShowNextStairs();
	//다음징검다리 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	ATA_StairActor* NextStair;
	//추가할 징검다리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	TArray<ATA_StairActor*> AdditionalStair;
	//끄고, 충돌 비활성화
	void DestoryCurrentStair();

	void MoveStair();
	void ResetStair();

	bool bIsPlayerOnStair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TimeBeforeMovingDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	EstairMovementType MovementType;
private:
	
	FTimerHandle DestroyTimeHandle;
	FTimerHandle MoveTimerHandle;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;

	FVector InitialPosition;
	FVector TargetPosition;

	bool bIsMovingUp;
	bool bIsMovingForward;

	
};
