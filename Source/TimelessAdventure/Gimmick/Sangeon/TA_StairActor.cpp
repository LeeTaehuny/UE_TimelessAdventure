// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Sangeon/TA_StairActor.h"

#include "MaterialHLSLTree.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

// Sets default values
ATA_StairActor::ATA_StairActor()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATA_StairActor::OnPlayerStep);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	bIsMovingForward = false;
	bIsMovingUp = false;
	bIsPlayerOnStair = false;
	
	MoveHeight = 100.0f;
	MoveSpeed = 20.0f;
	MoveDistance = 100.0f;
	TimeBeforeMovingDown = 2.0f;

	MovementType = EstairMovementType::None;
	
	if(UStaticMeshComponent* Mesh = FindComponentByClass<UStaticMeshComponent>())
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCollisionProfileName("BlockAll");
	}
	
	
}

// Called when the game starts or when spawned
void ATA_StairActor::BeginPlay()
{
	Super::BeginPlay();
	InitialPosition = GetActorLocation();

}

// Called every frame
void ATA_StairActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsPlayerOnStair)
	{
		MoveStair();
	}
}

void ATA_StairActor::OnPlayerStep(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA(ACharacter::StaticClass()))
	{
		ShowNextStairs();
		bIsMovingForward = true;
		
		switch(MovementType)
		{
		case EstairMovementType::None:
			GetWorld()->GetTimerManager().SetTimer(DestroyTimeHandle, this, &ATA_StairActor::DestoryCurrentStair, 2.0f, false);
			break;

		case EstairMovementType::Vertical:
			bIsMovingUp = true;
			bIsPlayerOnStair = true;
			GetWorld()->GetTimerManager().SetTimer(DestroyTimeHandle, this, &ATA_StairActor::DestoryCurrentStair, 5.0f, false);

			break;
		case EstairMovementType::Horizontal:
			bIsMovingForward = true;
			bIsPlayerOnStair = true;
			GetWorld()->GetTimerManager().SetTimer(DestroyTimeHandle, this, &ATA_StairActor::DestoryCurrentStair, 5.0f, false);

			break;
		default:
			break;
		}
	}
}

void ATA_StairActor::ShowNextStairs()
{
	if(NextStair)
	{
		NextStair->SetActorHiddenInGame(false);
		NextStair->SetActorEnableCollision(true);
	}

	for(int32 i = 0; i < AdditionalStair.Num(); i++)
	{
		if(AdditionalStair[i])
		{
			AdditionalStair[i]->SetActorHiddenInGame(false);
			AdditionalStair[i]->SetActorEnableCollision(true);
		}
	}
	
}

void ATA_StairActor::DestoryCurrentStair()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ATA_StairActor::MoveStair()
{
	FVector CurrentPosition = GetActorLocation();
	FVector NewPosition;

	if(MovementType == EstairMovementType::Vertical)
	{
		if(bIsMovingUp)
		{
			NewPosition = FMath::VInterpTo(CurrentPosition, InitialPosition + FVector(0,0,MoveHeight), GetWorld()->GetDeltaSeconds(),MoveSpeed);
			if(FVector::Dist(NewPosition, InitialPosition + FVector(0,0,MoveHeight)) < 1.0f)
			{
				bIsMovingUp = false;
			}
		}
		else
		{
			NewPosition = FMath::VInterpTo(CurrentPosition, InitialPosition, GetWorld()->GetDeltaSeconds(), MoveSpeed);
			if(FVector::Dist(NewPosition, InitialPosition) < 1.0f)
			{
				bIsMovingUp = true;
			}
		}
	}
	else if(MovementType == EstairMovementType::Horizontal)
	{
		if(bIsMovingForward)
		{
			NewPosition = FMath::VInterpTo(CurrentPosition, InitialPosition + FVector(MoveDistance, 0, 0), GetWorld()->GetDeltaSeconds(), MoveSpeed);
			if(FVector::Dist(NewPosition, InitialPosition + FVector(MoveDistance, 0, 0)) < 1.0f)
			{
				bIsMovingForward = false;
			}
		}
		else
		{
			NewPosition = FMath::VInterpTo(CurrentPosition, InitialPosition, GetWorld()->GetDeltaSeconds(), MoveSpeed);
			if(FVector::Dist(NewPosition, InitialPosition) < 1.0f)
			{
				bIsMovingForward = true;
			}
		}
	}
	SetActorLocation(NewPosition);
}

