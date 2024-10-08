// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TA_EndingActor.h"
#include "Interface/CombatComponentInterface.h"
#include "Component/TA_CombatComponent.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ATA_EndingActor::ATA_EndingActor()
{
 	PrimaryActorTick.bCanEverTick = true;

	InteractionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollider"));
	SetRootComponent(InteractionCollider);
	InteractionCollider->SetCollisionProfileName(TEXT("Item"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(InteractionCollider);

	Crystal1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crystal1"));
	Crystal1->SetupAttachment(InteractionCollider);

	Crystal2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crystal2"));
	Crystal2->SetupAttachment(InteractionCollider);

	Crystal3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crystal3"));
	Crystal3->SetupAttachment(InteractionCollider);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(InteractionCollider);

	bIsOpen = false;

	bFirstVisible = false;
	bSecondVisible = false;
	bThirdVisible = false;
}

void ATA_EndingActor::BeginPlay()
{
	Super::BeginPlay();
	
	Crystal1->SetVisibility(false);
	Crystal2->SetVisibility(false);
	Crystal3->SetVisibility(false);

	DoorWorldLocation = GetTransform().TransformPosition(Door->GetRelativeLocation());
}

void ATA_EndingActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsOpen)
	{
		FVector NewDoorWorldLocation = GetTransform().TransformPosition(Door->GetRelativeLocation()) + GetActorForwardVector() * 200.0f * DeltaSeconds;
		Door->SetWorldLocation(NewDoorWorldLocation);

		if (FVector::Distance(DoorWorldLocation, NewDoorWorldLocation) >= 10000.0f)
		{
			bIsOpen = false;
		}
	}
}

void ATA_EndingActor::Interaction(ACharacter* Target)
{
	if (!Target && !(Target->GetMesh()->GetAnimInstance())) return;

	if (bFirstVisible && bSecondVisible && bThirdVisible) return;

	// Combat ������Ʈ�� �����ϴ� ���
	if (ICombatComponentInterface* CombatInterface = Cast<ICombatComponentInterface>(Target))
	{
		Target->GetMesh()->GetAnimInstance()->Montage_Play(CombatInterface->GetCombatComponent()->GetPickupMontage(), 0.3f);

		if (!bFirstVisible)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATA_EndingActor::VisibleFirstCrystal, 0.7f, false);
			bFirstVisible = true;
		}
		else if (!bSecondVisible)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATA_EndingActor::VisibleSecondCrystal, 0.7f, false);
			Target->AddActorLocalRotation(FRotator(0.0f, 30.0f, 0.0f));
			bSecondVisible = true;
		}
		else
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATA_EndingActor::VisibleThirdCrystal, 0.7f, false);
			Target->AddActorLocalRotation(FRotator(0.0f, 30.0f, 0.0f));
			bThirdVisible = true;
		}
	}
}

void ATA_EndingActor::OpenDoor()
{
	bIsOpen = true;
}

void ATA_EndingActor::VisibleFirstCrystal()
{
	Crystal1->SetVisibility(true);
}

void ATA_EndingActor::VisibleSecondCrystal()
{
	Crystal2->SetVisibility(true);
}

void ATA_EndingActor::VisibleThirdCrystal()
{
	Crystal3->SetVisibility(true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATA_EndingActor::OpenDoor, 2.0f, false);
}

