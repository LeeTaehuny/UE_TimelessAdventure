// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Sangeon/TA_ButtonActor.h"
#include "Gimmick/Sangeon/TA_StairActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ATA_ButtonActor::ATA_ButtonActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	// 트리거 박스 생성
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATA_ButtonActor::OnPlayerOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATA_ButtonActor::OnPlayerOverlapEnd);

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(TriggerBox);
	
	FirstStair =  nullptr;
}

// Called when the game starts or when spawned
void ATA_ButtonActor::BeginPlay()
{
	Super::BeginPlay();
	InitialPosition = GetActorLocation();
	PressedPosition = InitialPosition - FVector(0, 0, 15.0f);
	
}

// Called every frame
void ATA_ButtonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void ATA_ButtonActor::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA(ACharacter::StaticClass()))
	{
		MoveButton(PressedPosition);

		if(FirstStair)
		{
			FirstStair->SetActorHiddenInGame(false);
			FirstStair->SetActorEnableCollision(true);
		}
	}
}

void ATA_ButtonActor::OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->IsA(ACharacter::StaticClass()))
	{
		MoveButton(InitialPosition);
	}
}

void ATA_ButtonActor::MoveButton(FVector TargetPosition)
{
	GetWorld()->GetTimerManager().SetTimer(ButtonMoveTimer, [this, TargetPosition]()
	{
		FVector CurrentPosition = GetActorLocation();
		FVector NewPosition = FMath::VInterpTo(CurrentPosition, TargetPosition, GetWorld()->GetDeltaSeconds(), 10.0f);

		SetActorLocation(NewPosition);

		if(FVector::Dist(CurrentPosition, TargetPosition) < 0.1f)
		{
			GetWorld()->GetTimerManager().ClearTimer(ButtonMoveTimer);
		}
	}, 0.01f, true);
}