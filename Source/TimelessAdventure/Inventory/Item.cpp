// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "TimelessAdventure/TA_PlayerCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMeshComponent;
}

void AItem::OnPickUp(class ATA_PlayerCharacter* Player)
{
	
	if(Player)
	{
		//Player->AddItemToInventory(ItemData);
		Destroy();
	}
}

void AItem::OnDrop(class ATA_PlayerCharacter* Player)
{
	if(Player)
	{
		FVector DropLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 100.0f;
		AItem* DroppedItem = GetWorld()->SpawnActor<AItem>(GetClass(), DropLocation, FRotator::ZeroRotator);
		DroppedItem->ItemData = ItemData;
		//Player->RemoveItemFromInventory(ItemData, ItemID);
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

