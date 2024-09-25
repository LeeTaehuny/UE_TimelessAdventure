// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TA_Slot.h"
#include "Interface/InventoryInterface.h"
#include "Component/TA_InventoryComponent.h"
#include "UI/TA_DragSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UTA_Slot::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UTA_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 우클릭 입력이 들어온 경우
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
	}
	// 좌클릭 입력이 들어온 경우
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// 정보 체크용 변수
		bool Success = false;

		// 해당 슬롯에 아이템이 존재하는지 체크합니다.
		IInventoryInterface* InvInterface = Cast<IInventoryInterface>(OwnerActor);
		if (InvInterface)
		{
			switch (SlotType)
			{
			case ESlotType::ST_Inventory_C:
				if (IsValid(InvInterface->GetInventory()) && !InvInterface->GetInventory()->GetCInventory()[SlotNum].bIsEmpty)
				{
					Success = true;
				}
				break;
			case ESlotType::ST_Inventory_M:
				if (IsValid(InvInterface->GetInventory()) && !InvInterface->GetInventory()->GetMInventory()[SlotNum].bIsEmpty)
				{
					Success = true;
				}
				break;
			case ESlotType::ST_QuickSlot:
				break;
			default:
				break;
			}
		}

		// 유효한 아이템이 존재하는 경우
		if (Success)
		{
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}

	return Reply.NativeReply;
}

void UTA_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	// 드래그가 시작될 때 호출되는 함수
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 드래그 오퍼가 비어있는 경우
	if (OutOperation == nullptr)
	{
		// 드래그 슬롯을 생성합니다.
		UTA_DragSlot* Operation = NewObject<UTA_DragSlot>();
		OutOperation = Operation;

		// 슬롯과 슬롯 타입을 지정합니다.
		Operation->PrevSlotIndex = SlotNum;
		Operation->SlotType = SlotType;

		// 드래그 시 출력될 위젯을 스폰합니다. (단지 출력 용도)
		if (DragSlotClass)
		{
			UTA_Slot* DragSlot = CreateWidget<UTA_Slot>(GetWorld(), DragSlotClass);
			if (DragSlot)
			{
				// 초기화
				DragSlot->SetOwnerPlayer(OwnerActor);
				DragSlot->SetNum(SlotNum);
				DragSlot->Init(SlotType);
				DragSlot->UpdateSlot();

				// 드래그 슬롯의 드래그 위젯으로 설정합니다.
				Operation->DefaultDragVisual = DragSlot;
			}
		}
	}
}

bool UTA_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 드래그가 종료될 때 호출되는 함수
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// 전달받은 InOperation(UTA_DragSlot)에 저장된 값을 활용해 현재 슬롯과 비교합니다.
	UTA_DragSlot* DragSlot = Cast<UTA_DragSlot>(InOperation);
	if (DragSlot)
	{
		// 슬롯의 타입이 같은 경우
		if (DragSlot->SlotType == SlotType)
		{
			// 인벤토리 내부 위치를 교체합니다.
			IInventoryInterface* InvInterface = Cast<IInventoryInterface>(OwnerActor);
			if (InvInterface)
			{
				InvInterface->GetInventory()->SwapItem(DragSlot->SlotType, DragSlot->PrevSlotIndex, SlotType, SlotNum);
			}
		}
	}

	return false;
}

void UTA_Slot::Init(ESlotType Type)
{
	SlotType = Type;
}

void UTA_Slot::UpdateSlot()
{
	if (!OwnerActor) return;
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
				TXT_Quantity->SetText(FText::FromString(FString::FromInt(InvInterface->GetInventory()->GetMInventory()[SlotNum].Quantity)));
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
