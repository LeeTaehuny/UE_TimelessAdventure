// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_Slot.h"
#include "Interface/InventoryInterface.h"
#include "Component/TA_InventoryComponent.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTA_Slot::NativeConstruct()
{
}

FReply UTA_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 우클릭 입력이 들어온 경우
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("Left"));
	}
	// 좌클릭 입력이 들어온 경우
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("Right"));
	}

	return Reply.NativeReply;
}

void UTA_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UTA_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UTA_Slot::Init(ESlotType Type)
{
	SlotType = Type;
}

void UTA_Slot::UpdateSlot()
{
	if (!OwnerActor) return;
	UE_LOG(LogTemp, Warning, TEXT("Update"));
	IInventoryInterface* InvInterface = Cast<IInventoryInterface>(OwnerActor);
	if (InvInterface)
	{
		switch (SlotType)
		{
		case ESlotType::ST_Inventory_C:
			// 해당 슬롯의 정보가 유효한 경우
			if (InvInterface->GetInventory()->GetCInventory().IsValidIndex(SlotNum) && !InvInterface->GetInventory()->GetCInventory()[SlotNum].bIsEmpty)
			{
				// 해당 텍스처 설정
				IMG_ItemIcon->SetBrushFromTexture(InvInterface->GetInventory()->GetCInventory()[SlotNum].Data.ItemThumbnail);
				// 수량 설정
				TXT_Quantity->SetText(FText::FromString(FString::FromInt(InvInterface->GetInventory()->GetCInventory()[SlotNum].Quantity)));
			}
			else
			{
				// 빈 텍스처 설정
				IMG_ItemIcon->SetBrushFromTexture(NoneTexture);
				// 수량 설정
				TXT_Quantity->SetText(FText::FromString(TEXT("")));
			}
			break;
		case ESlotType::ST_Inventory_M:
			// 해당 슬롯의 정보가 유효한 경우
			if (InvInterface->GetInventory()->GetMInventory().IsValidIndex(SlotNum) && !InvInterface->GetInventory()->GetMInventory()[SlotNum].bIsEmpty)
			{
				// 해당 텍스처 설정
				IMG_ItemIcon->SetBrushFromTexture(InvInterface->GetInventory()->GetMInventory()[SlotNum].Data.ItemThumbnail);
				// 수량 설정
				TXT_Quantity->SetText(FText::FromString(FString::FromInt(InvInterface->GetInventory()->GetCInventory()[SlotNum].Quantity)));
			}
			else
			{
				// 빈 텍스처 설정
				IMG_ItemIcon->SetBrushFromTexture(NoneTexture);
				// 수량 설정
				TXT_Quantity->SetText(FText::FromString(TEXT("")));
			}
			break;
		case ESlotType::ST_QuickSlot:
			break;
		default:
			break;
		}
	}
	
}
