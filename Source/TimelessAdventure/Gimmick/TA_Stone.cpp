// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TA_Stone.h"
#include "Player/TA_PlayerController.h"
#include "UI/TA_StoneMenu.h"
#include "Game/TA_GameInstance.h"

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
	
	if (ShopItems.Num())
	{
		// 해당 이름의 아이템 데이터 가져오기
		UTA_GameInstance* GI = Cast<UTA_GameInstance>(GetWorld()->GetGameInstance());
		if (!GI) return;

		for (int32 i = 0; i < ShopItems.Num(); i++)
		{
			FItemData ItemData = GI->GetItemData(ShopItems[i]);
			// 상점 아이템 데이터 목록에 추가
			ShopItemData.Add(ItemData);
		}
	}

	StoneWidget = CreateWidget<UTA_StoneMenu>(GetWorld(), StoneWidgetClass);
	if (StoneWidget)
	{
		StoneWidget->SetOwnerPlayer(this);
		StoneWidget->Init();
		StoneWidget->AddToViewport();
		StoneWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATA_Stone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &ATA_Stone::OnComponentBeginOverlap);
	InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &ATA_Stone::OnComponentEndOverlap);
}

void ATA_Stone::Interaction(ACharacter* Target)
{
	ATA_PlayerController* PC = Cast<ATA_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && !PC->GetInventoryVisible())
	{
		SetVisibilityStoneMenu(true);
	}
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
			if (StoneWidget->IsVisible())
			{
				SetVisibilityStoneMenu(false);
			}
		}
	}
}

void ATA_Stone::SetVisibilityStoneMenu(bool Value)
{
	if (StoneWidget)
	{
		ATA_PlayerController* PC = Cast<ATA_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (!PC) return;

		if (Value)
		{
			StoneWidget->UpdateWidget();
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeGameAndUI());
			StoneWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
			StoneWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

