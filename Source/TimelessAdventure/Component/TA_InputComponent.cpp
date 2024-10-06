// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_InputComponent.h"
#include "TA_CombatComponent.h"
#include "TA_InventoryComponent.h"
#include "Player/TA_PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

#include "HR/HR_StopAbilityComponent_T.h"

#include "Gimmick/Sangeon/TA_GrapRotateComponent.h"
#include "Player/TA_PlayerController.h"


UTA_InputComponent::UTA_InputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PlayerState = EPlayerState::PS_Combat;
}

void UTA_InputComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(OwnerPlayer)) return;

	// InputMappingContext 설정
	APlayerController* PlayerController = Cast<APlayerController>(OwnerPlayer->GetController());
	if (PlayerController && IMC_Player)
	{
		// Subsystem 받아오기
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add Mapping Context
			SubSystem->AddMappingContext(IMC_Player, 0);
			// Input Start
			OwnerPlayer->EnableInput(PlayerController);
		}
	}
}

void UTA_InputComponent::AddInput(UInputComponent* PlayerInputComponent)
{
	if (!IsValid(OwnerPlayer)) return;

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicLook);
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicMove);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UTA_InputComponent::BasicJump);
	EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicRoll);
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Started, this, &UTA_InputComponent::DashStart);
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Completed, this, &UTA_InputComponent::DashEnd);
	EnhancedInputComponent->BindAction(IA_LeftClick, ETriggerEvent::Started, this, &UTA_InputComponent::LeftClick);
	EnhancedInputComponent->BindAction(IA_RightClick, ETriggerEvent::Started, this, &UTA_InputComponent::RightClickStart);
	EnhancedInputComponent->BindAction(IA_RightClick, ETriggerEvent::Completed, this, &UTA_InputComponent::RightClickEnd);
	EnhancedInputComponent->BindAction(IA_MiddleClick, ETriggerEvent::Started, this, &UTA_InputComponent::MiddleClickStart);
	EnhancedInputComponent->BindAction(IA_MiddleClick, ETriggerEvent::Completed, this, &UTA_InputComponent::MiddleClickEnd);
	EnhancedInputComponent->BindAction(IA_ConvertInventory, ETriggerEvent::Started, this, &UTA_InputComponent::ConvertInventory);
	EnhancedInputComponent->BindAction(IA_Num1, ETriggerEvent::Started, this, &UTA_InputComponent::NumClick, 1);
	EnhancedInputComponent->BindAction(IA_Num2, ETriggerEvent::Started, this, &UTA_InputComponent::NumClick, 2);
	EnhancedInputComponent->BindAction(IA_Interaction, ETriggerEvent::Started, this, &UTA_InputComponent::Interaction);
	EnhancedInputComponent->BindAction(IA_Tab, ETriggerEvent::Started, this, &UTA_InputComponent::TabClick);	
	EnhancedInputComponent->BindAction(IA_Grap, ETriggerEvent::Triggered, this, &UTA_InputComponent::Grap);
	EnhancedInputComponent->BindAction(IA_Rotate, ETriggerEvent::Triggered, this, &UTA_InputComponent::Rotate);
	EnhancedInputComponent->BindAction(IA_z, ETriggerEvent::Started, this, &UTA_InputComponent::SetZ);
	EnhancedInputComponent->BindAction(IA_z, ETriggerEvent::Completed, this, &UTA_InputComponent::SetZfalse);
	EnhancedInputComponent->BindAction(IA_x, ETriggerEvent::Started, this, &UTA_InputComponent::SetX);
	EnhancedInputComponent->BindAction(IA_x, ETriggerEvent::Completed, this, &UTA_InputComponent::SetXfalse);

	
	
}

void UTA_InputComponent::BasicMove(const FInputActionValue& Value)
{
	if (!IsValid(OwnerPlayer->GetCombatComponent())) return;

	MovementVector = Value.Get<FVector2D>();

	// Controller rotation Yaw값 저장
	const FRotator Rotation = OwnerPlayer->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Yaw값을 기준으로 전방과 우측 방향 가져오기 (Y: forward, X : right)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	OwnerPlayer->GetCombatComponent()->Walk(ForwardDirection, RightDirection, MovementVector);
}

void UTA_InputComponent::BasicLook(const FInputActionValue& Value)
{
	if (!IsValid(OwnerPlayer)) return;

	FVector2D LookVector = Value.Get<FVector2D>();
	
	OwnerPlayer->AddControllerYawInput(LookVector.X);
	OwnerPlayer->AddControllerPitchInput(LookVector.Y * -1.0f);
}

void UTA_InputComponent::DashStart()
{
	if (!IsValid(OwnerPlayer->GetCombatComponent())) return;

	OwnerPlayer->GetCombatComponent()->DashStart();
}

void UTA_InputComponent::DashEnd()
{
	if (!IsValid(OwnerPlayer->GetCombatComponent())) return;

	OwnerPlayer->GetCombatComponent()->DashEnd();
}

void UTA_InputComponent::BasicRoll()
{
	if (!IsValid(OwnerPlayer->GetCombatComponent())) return;

	OwnerPlayer->GetCombatComponent()->RollStart(MovementVector);
}

void UTA_InputComponent::BasicJump()
{
	if (!IsValid(OwnerPlayer->GetCombatComponent())) return;

	OwnerPlayer->GetCombatComponent()->CombatJump();
}

void UTA_InputComponent::LeftClick()
{
	
	switch (PlayerState)
	{
	case EPlayerState::PS_Combat:
		if (IsValid(OwnerPlayer->GetCombatComponent())) OwnerPlayer->GetCombatComponent()->Attack();
		break;
	case EPlayerState::PS_StopObject:
		//if(IsValid(OwnerPlayer->GetStopAbilityComponent())) (OwnerPlayer->GetStopAbilityComponent())->StopAbilityEnd();
		//PlayerState = EPlayerState::PS_Combat;
		break;
	}
}

void UTA_InputComponent::RightClickStart()
{
	switch (PlayerState)
	{
	case EPlayerState::PS_Combat:
		if (IsValid(OwnerPlayer->GetCombatComponent())) OwnerPlayer->GetCombatComponent()->RightClickStart();
		break;
	}
}

void UTA_InputComponent::RightClickEnd()
{
	switch (PlayerState)
	{
	case EPlayerState::PS_Combat:
		if (IsValid(OwnerPlayer->GetCombatComponent())) OwnerPlayer->GetCombatComponent()->RightClickEnd();
		break;
	}
}

void UTA_InputComponent::MiddleClickStart()
{
	switch (PlayerState)
	{
	case EPlayerState::PS_Combat:
		if (IsValid(OwnerPlayer->GetCombatComponent())) OwnerPlayer->GetCombatComponent()->MiddleClickStart();
		break;
	}
}

void UTA_InputComponent::MiddleClickEnd()
{
	switch (PlayerState)
	{
	case EPlayerState::PS_Combat:
		if (IsValid(OwnerPlayer->GetCombatComponent())) OwnerPlayer->GetCombatComponent()->MiddleClickEnd();
		break;
	}
}

void UTA_InputComponent::ConvertInventory()
{
	switch (PlayerState)
	{
	case EPlayerState::PS_Combat:
		if (IsValid(OwnerPlayer->GetInventoryComponent())) OwnerPlayer->GetInventoryComponent()->ConvertInventory();
		break;
	}
}

void UTA_InputComponent::NumClick(int32 ClickNum)
{
	switch (PlayerState)
	{
	case EPlayerState::PS_Combat:
		if (IsValid(OwnerPlayer->GetInventoryComponent())) OwnerPlayer->GetInventoryComponent()->UseQuickSlot(ClickNum);
		break;
	}
}

void UTA_InputComponent::Interaction()
{
	switch (PlayerState)
	{
	case EPlayerState::PS_Combat:
		if (IsValid(OwnerPlayer->GetCombatComponent())) OwnerPlayer->GetCombatComponent()->Interaction();
		break;
	}
}


void UTA_InputComponent::TabClick()
{
	// UI 띄우기
	UE_LOG(LogTemp, Warning, TEXT("Input TabClick"));
	ATA_PlayerController* PC = Cast<ATA_PlayerController>(OwnerPlayer->GetController());
	if (PC)
	{
		PC->SetMouseLocation(640, 360);
		if(!bIsTab)
		{
			// Tab 처음이면 위젯 띄우기
			PC->bShowMouseCursor = true;
			PC->SetVisibleStateChangeWidget(true);
			bIsTab = true;
		}
		else
		{
			// 위젯 지우기
			PC->bShowMouseCursor = false;
			PC->SetVisibleStateChangeWidget(false);
			bIsTab = false;
			
			if(PlayerState == EPlayerState::PS_StopObject)
			{
				// Stop Object mode end
				if(IsValid(OwnerPlayer->GetStopAbilityComponent())) (OwnerPlayer->GetStopAbilityComponent())->StopAbilityEnd();
			}
		}
		
	}
}

void UTA_InputComponent::Grap()
{
	if(PlayerState == EPlayerState::PS_Combat)
	{
		return;
	}
	else if(PlayerState == EPlayerState::PS_GrabObject)
	{
		if(IsValid(OwnerPlayer->GetGrapRotateComponent()))
		{
			if(OwnerPlayer->GetGrapRotateComponent()->HeldObject)
			{
				//UE_LOG(LogTemp, Warning, TEXT("drop"));
				OwnerPlayer->GetGrapRotateComponent()->DropObject();			
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("pickup"));

				OwnerPlayer->GetGrapRotateComponent()->PickupObject();
			}
		}
	}
	
}


void UTA_InputComponent::Rotate(const FInputActionValue& ActionValue)
{
	if(PlayerState == EPlayerState::PS_GrabObject)
	{
		float RotationValue = ActionValue.Get<float>();
		if(OwnerPlayer->GetGrapRotateComponent())
		{
			OwnerPlayer->GetGrapRotateComponent()->RotateObject(RotationValue * 5.0f);
		}
	}
}

void UTA_InputComponent::SetX()
{
	if(PlayerState == EPlayerState::PS_GrabObject)
	{
		if(OwnerPlayer->GetGrapRotateComponent())
		{
			OwnerPlayer->GetGrapRotateComponent()->bShiftHeld = true;
		}
	}
}
void UTA_InputComponent::SetXfalse()
{
	if(PlayerState == EPlayerState::PS_GrabObject)
	{
		if(OwnerPlayer->GetGrapRotateComponent())
		{
			OwnerPlayer->GetGrapRotateComponent()->bShiftHeld = false;
		}
	}
}
void UTA_InputComponent::SetZ()
{
	if(PlayerState == EPlayerState::PS_GrabObject)
	{
		if(OwnerPlayer->GetGrapRotateComponent())
		{
			OwnerPlayer->GetGrapRotateComponent()->bctrlheld = true;
		}
	}
}

void UTA_InputComponent::SetZfalse()
{
	if(PlayerState == EPlayerState::PS_GrabObject)
	{
		if(OwnerPlayer->GetGrapRotateComponent())
		{
			OwnerPlayer->GetGrapRotateComponent()->bctrlheld = false;
		}
	}
}

void UTA_InputComponent::ChangeStateToCombat()
{
	PlayerState = EPlayerState::PS_Combat;
}

void UTA_InputComponent::ChangeStateToStopObject()
{
	PlayerState = EPlayerState::PS_StopObject;
}

void UTA_InputComponent::ChangeStateToGrabObject()
{
	PlayerState = EPlayerState::PS_GrabObject;
}

void UTA_InputComponent::ChangeState(EPlayerState NewState)
{
	//if (PlayerState == NewState) return;
	
	// 새로 들어온 상태에 따라 처리
	switch (NewState)
	{
	case EPlayerState::PS_Combat:
		UE_LOG(LogTemp, Display, TEXT("Combat"));

		PlayerState = NewState;
		break;
	case EPlayerState::PS_GrabObject:
		UE_LOG(LogTemp, Display, TEXT("Gimmick"));
		PlayerState = NewState;

		if(IsValid(OwnerPlayer->GetGrapRotateComponent()))
		{
			OwnerPlayer->GetGrapRotateComponent()->updatewidget();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("err"));
		}
		break;
	}
	
	PlayerState = NewState;
}
