// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TA_InventoryComponent.h"
#include "Component/TA_CombatComponent.h"
#include "Player/TA_PlayerController.h"
#include "Player/TA_PlayerCharacter.h"
#include "Interface/CombatComponentInterface.h"

#include "Game/TA_GameInstance.h"

UTA_InventoryComponent::UTA_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Inventory_C.Init(FInvItem(), 8);
	Inventory_M.Init(FInvItem(), 8);
	QuickSlot.Init(-1, 2);
}

void UTA_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// TEST
	int32 n1 = 1;
	int32 n2 = 1;
	AddItem(TEXT("B_Crystal"), n1);
	AddItem(TEXT("G_Crystal"), n2);
	AddGold(50);
}

void UTA_InventoryComponent::ConvertInventory()
{
	// 인벤토리 열기 / 닫기
	ATA_PlayerController* PC = Cast<ATA_PlayerController>(OwnerPlayer->GetController());
	if (PC)
	{
		PC->ConvertInventoryWidget();
	}
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

void UTA_InventoryComponent::UseQuickSlot(int32 Num)
{
	int32 Idx = Num - 1;

	// 해당 슬롯에 아이템이 존재하는 경우
	if (QuickSlot[Idx] != -1 && !Inventory_C[QuickSlot[Idx]].bIsEmpty)
	{
		// 아이템 사용
		UseItem(ESlotType::ST_Inventory_C, QuickSlot[Idx]);
	}
}

void UTA_InventoryComponent::UseItem(ESlotType Type, int32 Index)
{
	switch (Type)
	{
	case ESlotType::ST_Inventory_C:
		// 해당 아이템이 유효한지 체크
		if (Inventory_C.IsValidIndex(Index) && !Inventory_C[Index].bIsEmpty)
		{
			// 아이템 사용
			ICombatComponentInterface* CombatInterface = Cast<ICombatComponentInterface>(OwnerPlayer);
			if (CombatInterface)
			{
				CombatInterface->GetCombatComponent()->HealStat(Inventory_C[Index].Data.AddHPAmount, Inventory_C[Index].Data.AddStaminaAmount);
			}

			// 아이템 수량 감소
			Inventory_C[Index].Quantity--;
		
			OnChangeInventory.Broadcast();

			// 수량이 0인 경우
			if (Inventory_C[Index].Quantity <= 0)
			{
				// 아이템 제거
				RemoveItem(Type, Index);
			}
		}
		break;
	case ESlotType::ST_Inventory_M:
		// 해당 아이템이 유효한지 체크
		if (Inventory_M.IsValidIndex(Index) && !Inventory_M[Index].bIsEmpty)
		{
			// 아이템 수량 감소
			Inventory_M[Index].Quantity--;

			OnChangeInventory.Broadcast();

			// 수량이 0인 경우
			if (Inventory_M[Index].Quantity <= 0)
			{
				// 아이템 제거
				RemoveItem(Type, Index);
			}
		}
		break;
	}

}

void UTA_InventoryComponent::RemoveItem(ESlotType Type, int32 Index)
{
	switch (Type)
	{
	case ESlotType::ST_Inventory_C:
		if (Inventory_C.IsValidIndex(Index))
		{
			// 아이템 초기화
			Inventory_C[Index] = FInvItem();

			// 퀵 슬롯 확인 후 초기화
			for (int32 i = 0; i < QuickSlot.Num(); i++)
			{
				// 해당 인덱스를 가진 경우
				if (QuickSlot[i] == Index)
				{
					// 초기화
					QuickSlot[i] = -1;
				}
			}
		}
		break;
	case ESlotType::ST_Inventory_M:
		if (Inventory_M.IsValidIndex(Index))
		{
			// 아이템 초기화
			Inventory_M[Index] = FInvItem();
		}
		break;
	}

	OnChangeInventory.Broadcast();
}

void UTA_InventoryComponent::SwapItem(ESlotType Type1, int32 Index1, ESlotType Type2, int32 Index2)
{
	if (Type1 == Type2)
	{
		FInvItem Temp;

		switch (Type1)
		{
		case ESlotType::ST_Inventory_C:
			if (QuickSlot[0] == Index1 && QuickSlot[1] == Index2 || QuickSlot[0] == Index2 && QuickSlot[1] == Index1)
			{
				QuickSlot.Swap(0, 1);
			}
			else
			{
				for (int32 i = 0; i < QuickSlot.Num(); i++)
				{
					if (QuickSlot[i] == Index1)
					{
						QuickSlot[i] = Index2;
					}
				}
			}

			Temp = Inventory_C[Index1];
			Inventory_C[Index1] = Inventory_C[Index2];
			Inventory_C[Index2] = Temp;
			break;
		case ESlotType::ST_Inventory_M:
			Temp = Inventory_M[Index1];
			Inventory_M[Index1] = Inventory_M[Index2];
			Inventory_M[Index2] = Temp;
			break;
		case ESlotType::ST_QuickSlot:
			break;
		}
	}

	OnChangeInventory.Broadcast();
	for (FInvItem Item : Inventory_C)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *Item.Data.ItemName.ToString(), Item.Quantity);
	}
}

void UTA_InventoryComponent::AddQuickSlot(ESlotType Type, int32 Index1, int32 Index2)
{
	// 슬롯 타입이 소비 슬롯인 경우
	if (Type == ESlotType::ST_Inventory_C)
	{
		// 소비 슬롯의 해당 칸의 인덱스를 복제합니다.
		QuickSlot[Index2] = Index1;
	}

	OnChangeInventory.Broadcast();
}

void UTA_InventoryComponent::AddGold(int32 InGold)
{
	Gold += InGold;

	OnChangeGold.Broadcast();
}

bool UTA_InventoryComponent::PurchaseItem(FName ItemName, int32 Price)
{
	if (Gold >= Price)
	{
		int32 TempNum = 1;
		// 아이템 추가에 성공했다면?
		if (AddItem(ItemName, TempNum))
		{
			// 소지 골드 수정
			Gold -= Price;

			OnChangeGold.Broadcast();
			return true;
		}
	}

	return false;
}
