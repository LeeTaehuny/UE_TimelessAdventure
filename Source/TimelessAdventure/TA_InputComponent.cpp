// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_InputComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Animation/AnimMontage.h"
#include "Interface/PlayerComponentInterface.h"

UTA_InputComponent::UTA_InputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WalkSpeed = 300.0f;
	DashSpeed = 600.0f;
}

void UTA_InputComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(OwnerPlayer)) return;

	// InputMappingContext Setting
	APlayerController* PlayerController = Cast<APlayerController>(OwnerPlayer->GetController());
	if (PlayerController && IMC_Player)
	{
		// Call SubSystem
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add Mapping Context
			SubSystem->AddMappingContext(IMC_Player, 0);
			// Input Start
			OwnerPlayer->EnableInput(PlayerController);
		}
	}

	OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void UTA_InputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTA_InputComponent::AddInput(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicLook);
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicMove);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UTA_InputComponent::BasicJump);
	EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicRoll);
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Started, this, &UTA_InputComponent::DashStart);
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Completed, this, &UTA_InputComponent::DashEnd);
}

void UTA_InputComponent::BasicMove(const FInputActionValue& Value)
{
	if (!IsValid(OwnerPlayer)) return;

	// value extraction
	MovementVector = Value.Get<FVector2D>();

	// Controller rotation Yaw value
	const FRotator Rotation = OwnerPlayer->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward and right directions based on rotation(Yaw) (Y: forward, X : right)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Add Movement
	OwnerPlayer->AddMovementInput(ForwardDirection, MovementVector.X);
	OwnerPlayer->AddMovementInput(RightDirection, MovementVector.Y);
}

void UTA_InputComponent::BasicLook(const FInputActionValue& Value)
{
	if (!IsValid(OwnerPlayer)) return;

	// value extraction
	FVector2D LookVector = Value.Get<FVector2D>();
	
	// Add Controller Input
	OwnerPlayer->AddControllerYawInput(LookVector.X);
	OwnerPlayer->AddControllerPitchInput(LookVector.Y * -1.0f);
}

void UTA_InputComponent::DashStart()
{
	if (!IsValid(OwnerPlayer)) return;
	if (PlayerState == EPlayerState::PS_Ghost)
	{
		TempState = EPlayerState::PS_Dash;
		return;
	}
	if (PlayerState != EPlayerState::PS_Walk) return;

	// Set State (PS_Dush)
	ChangeState(EPlayerState::PS_Dash);
}

void UTA_InputComponent::DashEnd()
{
	if (!IsValid(OwnerPlayer)) return;
	if (PlayerState == EPlayerState::PS_Ghost)
	{
		TempState = EPlayerState::PS_Walk;
		return;
	}

	ChangeState(EPlayerState::PS_Walk);
}

void UTA_InputComponent::BasicRoll()
{
	if (!IsValid(OwnerPlayer)) return;
	if (PlayerState == EPlayerState::PS_Ghost) return;
	if (OwnerPlayer->GetCharacterMovement()->IsFalling()) return;
	if (PlayerState == EPlayerState::PS_Walk || PlayerState == EPlayerState::PS_Dash)
	{
		TempState = PlayerState;
	}

	ChangeState(EPlayerState::PS_Ghost);
	

	float Mult = TempState == EPlayerState::PS_Dash ? 1.3f : 1.0f;

	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// Controller rotation Yaw value
		const FRotator Rotation = OwnerPlayer->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward and right directions based on rotation(Yaw) (Y: forward, X : right)
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		FRotator TargetRot = (ForwardDirection * MovementVector.X + RightDirection * MovementVector.Y).Rotation();

		OwnerPlayer->SetActorRotation(TargetRot);

		AnimInstance->Montage_Play(RollMontage, Mult);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UTA_InputComponent::OnRollMontageEnd);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, RollMontage);
	}
}

void UTA_InputComponent::BasicJump()
{
	if (PlayerState == EPlayerState::PS_Ghost) return;

	if (IsValid(OwnerPlayer))
	{
		OwnerPlayer->Jump();
	}
}

void UTA_InputComponent::OnRollMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
		ChangeState(TempState);
}

void UTA_InputComponent::ChangeState(EPlayerState NewState)
{
	if (PlayerState == NewState) return;

	switch (NewState)
	{
	case EPlayerState::PS_Walk:
		OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	case EPlayerState::PS_Dash:
		OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
		break;
	case EPlayerState::PS_Combat:
		break;
	case EPlayerState::PS_Ghost:
		break;
	}

	PlayerState = NewState;
}

