// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Sangeon/TA_DeadZone.h"

#include "TA_StairActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATA_DeadZone::ATA_DeadZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	RootComponent = TriggerZone;
	TriggerZone->SetCollisionProfileName(TEXT("Trigger"));
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ATA_DeadZone::OnPlayerEnter);

}

// Called when the game starts or when spawned
void ATA_DeadZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATA_DeadZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATA_DeadZone::OnPlayerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA(ACharacter::StaticClass()))
	{
		TArray<AActor*> FoundStairs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATA_StairActor::StaticClass(), FoundStairs);
		for(AActor* StairActor : FoundStairs)
		{
			ATA_StairActor* Stair = Cast<ATA_StairActor>(StairActor);
			if(Stair)
			{
				Stair->SetActorHiddenInGame(true);
				Stair->SetActorEnableCollision(false);
			}
		}
	}
}
