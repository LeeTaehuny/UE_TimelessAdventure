// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TA_Stone.h"
#include "Player/TA_PlayerController.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ATA_Stone::ATA_Stone()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollider"));
	SetRootComponent(InteractionCollider);
	InteractionCollider->SetCollisionProfileName(TEXT("Item"));

	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComp"));
	ItemMeshComp->SetupAttachment(InteractionCollider);
}

void ATA_Stone::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATA_Stone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &ATA_Stone::OnComponentBeginOverlap);
	InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &ATA_Stone::OnComponentEndOverlap);
}

void ATA_Stone::Interaction(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("SSSSs"));
}

void ATA_Stone::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어인 경우
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == Cast<ACharacter>(OtherActor))
	{
		// 헬프 텍스트 출력
		ATA_PlayerController* PC = Cast<ATA_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			PC->SetInteractionText(InteractionText);
			PC->VisibleInteractionWidget(true);
		}
	}
}

void ATA_Stone::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 플레이어인 경우
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == Cast<ACharacter>(OtherActor))
	{
		// 헬프 텍스트 감추기
		ATA_PlayerController* PC = Cast<ATA_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			PC->VisibleInteractionWidget(false);
		}
	}
}

