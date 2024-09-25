// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TA_InventoryComponent.h"

#include "Game/TA_GameInstance.h"

UTA_InventoryComponent::UTA_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Inventory_C.Init(FInvItem(), 8);
	Inventory_M.Init(FInvItem(), 8);
}

void UTA_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	int32 num = 60;
	int32 num2 = 70;
	AddItem(TEXT("HPPotion"), num);
	AddItem(TEXT("HPPotion"), num2);

	for (FInvItem Item : Inventory_C)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *Item.Data.ItemName.ToString(), Item.Quantity);
	}
}

void UTA_InventoryComponent::ConvertInventory()
{
	// 인벤토리 열기 / 닫기
}

bool UTA_InventoryComponent::AddItem(FName ItemName, int32& Quantity)
{
	// 해당 이름의 아이템 데이터 가져오기
	UTA_GameInstance* GI = Cast<UTA_GameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return false;

	FItemData ItemData = GI->GetItemData(ItemName);

	// 인벤토리 아이템 생성
	FInvItem NewItem;
	NewItem.Data = ItemData;
	NewItem.Quantity = Quantity;
	NewItem.bIsEmpty = false;

	// 해당 아이템 타입에 따라 추가
	if (NewItem.Data.EItemType == EItemType::Consumable)
	{
		// 중첩 가능한 아이템인 경우
		if (NewItem.Data.bIsStackable)
		{
			for (FInvItem& InvItem : Inventory_C)
			{
				// 아이디가 일치하는 경우
				if (InvItem.Data.ItemID == NewItem.Data.ItemID)
				{
					// 이미 가득 차있으면 건너뛰기
					if (InvItem.Data.MaxStackCount == InvItem.Quantity) continue;

					// 아이템 남은 공간 계산 (남은 공간 : 최대 - 현재)
					int32 TempNum = InvItem.Data.MaxStackCount - InvItem.Quantity;
					// 남은 공간보다 새로 들어온 아이템의 수가 적은 경우
					if (TempNum >= NewItem.Quantity)
					{
						// 그대로 추가하고 반환
						InvItem.Quantity += NewItem.Quantity;
						OnChangeInventory.Broadcast();
						return true;
					}
					// 남은 공간보다 새로 들어온 아이템의 수가 많은 경우
					else
					{
						// 인벤토리 아이템을 최대 치로 설정
						InvItem.Quantity = InvItem.Data.MaxStackCount;
						// 남은 수량 업데이트
						NewItem.Quantity -= TempNum;
					}
				}
			}
		}

		// 빈 공간을 찾아서 남은 데이터 삽입
		for (int32 i = 0; i < Inventory_C.Num(); i++)
		{
			// 해당 인덱스가 비어있는(초기화) 경우
			if (Inventory_C[i].bIsEmpty)
			{
				Inventory_C[i] = NewItem;
				OnChangeInventory.Broadcast();
				return true;
			}
		}		
	}
	else if (NewItem.Data.EItemType == EItemType::Miscellaneous)
	{
		// 중첩 가능한 아이템인 경우
		if (NewItem.Data.bIsStackable)
		{
			for (FInvItem& InvItem : Inventory_M)
			{
				// 아이디가 일치하는 경우
				if (InvItem.Data.ItemID == NewItem.Data.ItemID)
				{
					// 이미 가득 차있으면 건너뛰기
					if (InvItem.Data.MaxStackCount == InvItem.Quantity) continue;

					// 아이템 남은 공간 계산 (남은 공간 : 최대 - 현재)
					int32 TempNum = InvItem.Data.MaxStackCount - InvItem.Quantity;
					// 남은 공간보다 새로 들어온 아이템의 수가 적은 경우
					if (TempNum >= NewItem.Quantity)
					{
						// 그대로 추가하고 반환
						InvItem.Quantity += NewItem.Quantity;
						OnChangeInventory.Broadcast();
						return true;
					}
					// 남은 공간보다 새로 들어온 아이템의 수가 많은 경우
					else
					{
						// 인벤토리 아이템을 최대 치로 설정
						InvItem.Quantity = InvItem.Data.MaxStackCount;
						// 남은 수량 업데이트
						NewItem.Quantity -= TempNum;
					}
				}
			}
		}

		// 빈 공간을 찾아서 남은 데이터 삽입
		for (int32 i = 0; i < Inventory_M.Num(); i++)
		{
			// 해당 인덱스가 비어있는(초기화) 경우
			if (Inventory_M[i].bIsEmpty)
			{
				Inventory_M[i] = NewItem;
				OnChangeInventory.Broadcast();
				return true;
			}
		}
	}

	// 남은 수량의 아이템 반환
	Quantity = NewItem.Quantity;

	// 여기까지 온 경우 인벤토리가 가득 차서 실패한 경우이므로 false 리턴
	OnChangeInventory.Broadcast();
	return false;
}

void UTA_InventoryComponent::UseItem(ESlotType Type, int32 Index)
{

}

void UTA_InventoryComponent::RemoveItem(ESlotType Type, int32 Index)
{

}

void UTA_InventoryComponent::SwapItem(ESlotType Type1, int32 Index1, ESlotType Type2, int32 Index2)
{

}