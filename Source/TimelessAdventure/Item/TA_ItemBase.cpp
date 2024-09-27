// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_ItemBase.h"
#include "Player/TA_PlayerController.h"
#include "Interface/CombatComponentInterface.h"
#include "Interface/InventoryInterface.h"
#include "Component/TA_CombatComponent.h"
#include "Component/TA_InventoryComponent.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATA_ItemBase::ATA_ItemBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	InteractionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollider"));
	SetRootComponent(InteractionCollider);
	InteractionCollider->SetCollisionProfileName(TEXT("Item"));

	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComp"));
	ItemMeshComp->SetupAttachment(InteractionCollider);
}

void ATA_ItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATA_ItemBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &ATA_ItemBase::OnComponentBeginOverlap);
	InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &ATA_ItemBase::OnComponentEndOverlap);
}

void ATA_ItemBase::Interaction(ACharacter* Target)
{
	if (!Target && !Target->GetMesh()->GetAnimInstance()) return;

	// Combat 컴포넌트가 존재하는 경우
	if (ICombatComponentInterface* CombatInterface = Cast<ICombatComponentInterface>(Target))
	{
		Target->GetMesh()->GetAnimInstance()->Montage_Play(CombatInterface->GetCombatComponent()->GetPickupMontage());
	}

	// Inventory 컴포넌트가 존재하는 경우
	if (IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(Target))
	{
		InventoryInterface->GetInventory()->AddItem(ItemName, Quantity);
		// 현재 아이템 제거
		Destroy();
	}
}

void ATA_ItemBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ATA_ItemBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

