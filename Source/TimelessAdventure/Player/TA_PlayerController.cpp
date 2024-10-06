// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TA_PlayerController.h"
#include "UI/TA_ChangeWeapon.h"
#include "UI/TA_HUD.h"
#include "Interface/InventoryInterface.h"
#include "Component/TA_InventoryComponent.h"
#include "UI/HR_StateChange.h"


ATA_PlayerController::ATA_PlayerController()
{
}

void ATA_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponSelectWidgetClass)
	{
		WeaponSelectWidget = CreateWidget<UTA_ChangeWeapon>(GetWorld(), WeaponSelectWidgetClass);
		if (WeaponSelectWidget)
		{
			WeaponSelectWidget->SetOwnerPlayer(GetPawn());
			WeaponSelectWidget->AddToViewport();
			VisibleWeaponSelectWidget(false);
		}
	}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UTA_HUD>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->SetOwnerPlayer(GetPawn());
			HUDWidget->SetAimVisibility(false);
			HUDWidget->Init();
			HUDWidget->AddToViewport();
		

			IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(GetPawn());
			if (!InventoryInterface) return;

			InventoryInterface->GetInventory()->OnChangeInventory.AddUObject(HUDWidget, &UTA_HUD::UpdateInventory);
			InventoryInterface->GetInventory()->OnChangeGold.AddUObject(HUDWidget, &UTA_HUD::UpdateGold);
		}
	}

	if(StateChangeWidgetClass)
	{
		StateChangeWidget = CreateWidget<UHR_StateChange>(GetWorld(), StateChangeWidgetClass);
		if (StateChangeWidget)
		{
			StateChangeWidget->SetOwnerPlayer(GetPawn());
			StateChangeWidget->AddToViewport();
			SetVisibleStateChangeWidget(false);
		}
	}
}

void ATA_PlayerController::VisibleWeaponSelectWidget(bool Value)
{
	// 인벤토리 위젯이 켜져있다면 반환
	if (HUDWidget && HUDWidget->GetInventoryVisibility()) return;

	if (Value)
	{
		if (WeaponSelectWidget)
		{
			FVector2D ViewportSize;
			if (GEngine && GEngine->GameViewport)
			{
				GEngine->GameViewport->GetViewportSize(ViewportSize);
			}

			SetMouseLocation(ViewportSize.X / 2, ViewportSize.Y / 2);

			// 위젯 활성화
			WeaponSelectWidget->OpenWidget();
			WeaponSelectWidget->SetVisibility(ESlateVisibility::Visible);
			
			// 입력 모드 설정
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);

			// 마우스 커서 보이도록 설정
			bShowMouseCursor = true;
			
		}
	}
	else
	{
		if (WeaponSelectWidget)
		{
			// 위젯 비활성화
			WeaponSelectWidget->SetVisibility(ESlateVisibility::Hidden);

			// 인벤토리 위젯이 꺼져있는 경우
			if (HUDWidget && !HUDWidget->GetInventoryVisibility())
			{
				// 입력 모드 설정
				FInputModeGameOnly InputMode;
				SetInputMode(InputMode);

				// 마우스 커서 보이도록 설정
				bShowMouseCursor = false;
			}	
		}
	}
}

void ATA_PlayerController::ConvertInventoryWidget()
{
	// 무기 교체 위젯이 켜져있는 경우 반환
	if (WeaponSelectWidget && WeaponSelectWidget->IsVisible()) return;

	if (HUDWidget)
	{
		// 인벤토리가 켜져있는 경우
		if (HUDWidget->GetInventoryVisibility())
		{
			// 위젯을 숨깁니다.
			HUDWidget->SetInventoryVisibility(false);

			// 입력 모드 설정
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);

			// 마우스 커서 보이도록 설정
			bShowMouseCursor = false;
		}
		else
		{
			// 위젯을 노출시킵니다.
			HUDWidget->SetInventoryVisibility(true);

			// 입력 모드 설정
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);

			// 마우스 커서 보이도록 설정
			bShowMouseCursor = true;
		}
	}
}

void ATA_PlayerController::VisibleAimWidget(bool Value)
{
	if (HUDWidget)
	{
		HUDWidget->SetAimVisibility(Value);
	}
}

void ATA_PlayerController::VisibleInteractionWidget(bool Value)
{
	if (HUDWidget)
	{
		HUDWidget->SetInteractionVisibility(Value);
	}
}

void ATA_PlayerController::SetInteractionText(FText NewText)
{
	if (HUDWidget)
	{
		HUDWidget->SetInteractionText(NewText);
	}
}

bool ATA_PlayerController::GetInventoryVisible()
{
	return HUDWidget->GetInventoryVisibility();
}

void ATA_PlayerController::SetVisibleStateChangeWidget(bool Value)
{
	if (StateChangeWidget)
	{
		if(Value) {StateChangeWidget->SetVisibility(ESlateVisibility::Visible);}
		else StateChangeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

