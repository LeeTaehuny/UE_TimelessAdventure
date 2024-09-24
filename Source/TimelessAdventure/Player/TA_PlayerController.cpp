// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TA_PlayerController.h"
#include "UI/TA_ChangeWeapon.h"
#include "UI/TA_HUD.h"

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
		}
	}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UTA_HUD>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->SetOwnerPlayer(GetPawn());
			HUDWidget->SetAimVisibility(false);
			HUDWidget->AddToViewport();
		}
	}
}

void ATA_PlayerController::VisibleWeaponSelectWidget(bool Value)
{
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
			WeaponSelectWidget->AddToViewport();

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
			WeaponSelectWidget->RemoveFromParent();

			// 입력 모드 설정
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);

			// 마우스 커서 보이도록 설정
			bShowMouseCursor = false;
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
