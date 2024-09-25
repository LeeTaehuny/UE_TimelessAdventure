// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_InputComponent.h"
#include "TA_CombatComponent.h"
#include "TA_InventoryComponent.h"
#include "Player/TA_PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

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

void UTA_InputComponent::ChangeState(EPlayerState NewState)
{
	if (PlayerState == NewState) return;
	
	// 새로 들어온 상태에 따라 처리
	switch (NewState)
	{
	case EPlayerState::PS_Combat:
		break;
	}
	
	PlayerState = NewState;
}
