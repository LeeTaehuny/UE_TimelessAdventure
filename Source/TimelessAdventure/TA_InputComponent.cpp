// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_InputComponent.h"
#include "TA_CombatComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "TA_EquipmentWidget.h"
#include "TA_InteractionComponent.h"
#include "TA_PlayerCharacter.h"
#include "TA_WeaponComponent.h"
#include "TA_WeaponComponent_bow.h"
#include "TA_WeaponComponent_sword.h"
#include "TA_WeaponComponent_torch.h"
#include "Animation/AnimMontage.h"
#include "Blueprint/UserWidget.h"
#include "Interface/PlayerComponentInterface.h"
#include "Inventory/TA_InventoryComponent.h"

UTA_InputComponent::UTA_InputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	WalkSpeed = 300.0f;
	DashSpeed = 600.0f;
	RollStaminaPercent = 0.2f;
}

void UTA_InputComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerPlayer = Cast<ACharacter>(GetOwner());
}

void UTA_InputComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(OwnerPlayer)) return;

	if(WeaponSelectionWidgetClass)
	{
		WeaponSelectionWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WeaponSelectionWidgetClass);
		WeaponSelectionWidgetInstance->AddToViewport();
		WeaponSelectionWidgetInstance->SetVisibility(ESlateVisibility::Hidden);

	}

	
	// Set InputMappingContext
	APlayerController* PlayerController = Cast<APlayerController>(OwnerPlayer->GetController());
	if (PlayerController && IMC_Player)
	{
		// Subsystem
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add Mapping Context
			SubSystem->AddMappingContext(IMC_Player, 0);
			// Input Start
			OwnerPlayer->EnableInput(PlayerController);
		}
	}

	// Set player init walk max speed -> walk speed
	OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	// ZeroHealthDelegate - function mapping
	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		ComponentInterface->GetCombatComponent()->ZeroHealthDelegate.AddUObject(this, &UTA_InputComponent::DashEnd);
	}
	
}

void UTA_InputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UTA_InputComponent::AddInput(UInputComponent* PlayerInputComponent)
{
	if (!IsValid(OwnerPlayer)) return;

	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicLook);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicMove);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UTA_InputComponent::BasicJump);
		EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &UTA_InputComponent::BasicRoll);
		EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Started, this, &UTA_InputComponent::DashStart);
		EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Completed, this, &UTA_InputComponent::DashEnd);
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &UTA_InputComponent::Attack);
		EnhancedInputComponent->BindAction(IA_SwitchWeapon, ETriggerEvent::Started, this, &UTA_InputComponent::OpenWeaponSelection);
		EnhancedInputComponent->BindAction(IA_SwitchWeapon, ETriggerEvent::Completed, this, &UTA_InputComponent::CloseWeaponSelection);
		EnhancedInputComponent->BindAction(IA_Interaction, ETriggerEvent::Started, this, &UTA_InputComponent::InteractionPositive);
		EnhancedInputComponent->BindAction(IA_Inventory, ETriggerEvent::Started, this, &UTA_InputComponent::ToggleInventory);

	}
}

void UTA_InputComponent::BasicMove(const FInputActionValue& Value)
{
	if (!IsValid(OwnerPlayer)) return;

	MovementVector = Value.Get<FVector2D>();
	if(MovementVector != PreviousMovementVector)
	{
		// 공격 중에 WASD 입력이 들어오면 공격 취소
		if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
		{
			UTA_CombatComponent* CombatComponent = ComponentInterface->GetCombatComponent();
			if (CombatComponent && CombatComponent->CurrentWeapon)
			{
				if(UTA_WeaponComponent_sword* SwordWeapon = Cast<UTA_WeaponComponent_sword>(CombatComponent->CurrentWeapon))
				{
					SwordWeapon->CancelAttackAndMove();
				
				}
				if(UTA_WeaponComponent_torch* TorchWeapon = Cast<UTA_WeaponComponent_torch>(CombatComponent->CurrentWeapon))
				{
					TorchWeapon->CancelAttackAndMove();
				
				}
				if(UTA_WeaponComponent* Weapon = Cast<UTA_WeaponComponent>(CombatComponent->CurrentWeapon))
				{
					Weapon->CancelAttackAndMove();
				
				}
			}
		}
	}
	

	// Controller rotation Yaw값 저장
	const FRotator Rotation = OwnerPlayer->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// forward, right direction for Yaw value (Y: forward, X : right)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	OwnerPlayer->AddMovementInput(ForwardDirection, MovementVector.X);
	OwnerPlayer->AddMovementInput(RightDirection, MovementVector.Y);

	PreviousMovementVector = MovementVector;
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
	if (!IsValid(OwnerPlayer)) return;

	// dash start when roll state
	if (PlayerState == EPlayerState::PS_Roll)
	{
		// save dash state
		TempState = EPlayerState::PS_Dash;
		return;
	}
	// Can't dash when player's state not walk
	if (PlayerState != EPlayerState::PS_Walk) return;

	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		// Set Use Stamina persistence (Dash state)
		ComponentInterface->GetCombatComponent()->SetUseStamina(true);
	}

	// Change state (Dush)
	ChangeState(EPlayerState::PS_Dash);
}

void UTA_InputComponent::DashEnd()
{
	if (!IsValid(OwnerPlayer)) return;

	// Dash end when Rolling state
	if (PlayerState == EPlayerState::PS_Roll)
	{
		// save state for walk
		TempState = EPlayerState::PS_Walk;
		return;
	}

	// Change state (Walk)
	ChangeState(EPlayerState::PS_Walk);

	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		// Set not use stamina (Dash end)
		ComponentInterface->GetCombatComponent()->SetUseStamina(false);
	}
}

void UTA_InputComponent::BasicRoll()
{
	if (!IsValid(OwnerPlayer)) return;
	if (PlayerState == EPlayerState::PS_Roll) return;
	if (OwnerPlayer->GetCharacterMovement()->IsFalling()) return;

	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		// Rollstamina < Current stamina
		if (ComponentInterface->GetCombatComponent()->GetStaminaPercent() > RollStaminaPercent)
		{
			// Use Stamina
			ComponentInterface->GetCombatComponent()->UpdateStamina(true, RollStaminaPercent);

			// save player before state (Walk/Dash)
			if (PlayerState == EPlayerState::PS_Walk || PlayerState == EPlayerState::PS_Dash)
			{
				TempState = PlayerState;
			}

			// Change state (PS_Roll)
			ChangeState(EPlayerState::PS_Roll);

			// set rolling speed (walk : 1, dash : 1.3)
			float Mult = TempState == EPlayerState::PS_Dash ? 1.3f : 1.0f;

			UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				// save Controller rotation Yaw value
				const FRotator Rotation = OwnerPlayer->Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// save forward, right vector by Yaw value (Y: forward, X : right)
				const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

				// calculate rotation vector based on forward, right direction (input value)
				FRotator TargetRot = (ForwardDirection * MovementVector.X + RightDirection * MovementVector.Y).Rotation();

				// Rotation for last input
				OwnerPlayer->SetActorRotation(TargetRot);

				// play rolling montage
				AnimInstance->Montage_Play(RollMontage, Mult);

				// when rolling montage finished, binding function
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &UTA_InputComponent::OnRollMontageEnd);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, RollMontage);
			}
		}
	}
}

void UTA_InputComponent::BasicJump()
{
	if (PlayerState == EPlayerState::PS_Roll) return;

	if (IsValid(OwnerPlayer))
	{
		OwnerPlayer->Jump();
	}
}

void UTA_InputComponent::OpenWeaponSelection()
{
	if (!IsValid(OwnerPlayer) || !WeaponSelectionWidgetInstance) return	;
	//WeaponSelectionWidgetInstance->AddToViewport();
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	WeaponSelectionWidgetInstance->SetVisibility(ESlateVisibility::Visible);

	//OwnerPlayer->GetCharacterMovement()->DisableMovement();
}
void UTA_InputComponent::CloseWeaponSelection()
{

	if (!IsValid(OwnerPlayer) || !WeaponSelectionWidgetInstance) return;

	UTA_EquipmentWidget* EquipmentWidget = Cast<UTA_EquipmentWidget>(WeaponSelectionWidgetInstance);
	int32 HoveredWeapon = EquipmentWidget->CurrentHoveredWeapon;
//	WeaponSelectionWidgetInstance->RemoveFromViewport();
	WeaponSelectionWidgetInstance->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	SwitchWeapon(HoveredWeapon);

}


void UTA_InputComponent::ToggleInventory()
{
	ATA_PlayerCharacter* PlayerCharacter = Cast<ATA_PlayerCharacter>(GetOwner());
	UTA_InventoryComponent* InventoryComponent = OwnerPlayer->FindComponentByClass<UTA_InventoryComponent>();

	if(PlayerCharacter && InventoryComponent)
	{
		InventoryComponent->ToggleInventory();
	}
}

void UTA_InputComponent::SwitchWeapon(int32 equipweapon)
{
	if (!IsValid(OwnerPlayer)) return;

	// CombatComponent 및 PlayerComponentInterface 참조 확인
	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		UTA_CombatComponent* CombatComponent = ComponentInterface->GetCombatComponent();
		TArray<UTA_WeaponComponent*> WeaponList;
		WeaponList.Add(nullptr);
		WeaponList.Add(Cast<UTA_WeaponComponent>(OwnerPlayer->FindComponentByClass<UTA_WeaponComponent_sword>()));
		WeaponList.Add(Cast<UTA_WeaponComponent>(OwnerPlayer->FindComponentByClass<UTA_WeaponComponent_bow>()));
		WeaponList.Add(Cast<UTA_WeaponComponent>(OwnerPlayer->FindComponentByClass<UTA_WeaponComponent_torch>()));
		WeaponList.Add(Cast<UTA_WeaponComponent>(OwnerPlayer->FindComponentByClass<UTA_WeaponComponent>())); // Fist
		
		// 현재 무기 가져오기
		//UTA_WeaponComponent* CurrentWeapon = CombatComponent->CurrentWeapon;
		//int32 CurrentIndex = WeaponList.IndexOfByKey(CurrentWeapon);

		UTA_WeaponComponent* NextWeapon = WeaponList[equipweapon];
		CombatComponent->EquipWeapon(NextWeapon);
	}
}

void UTA_InputComponent::Attack()
{
	if (!IsValid(OwnerPlayer)) return;

	// CombatComponent 및 PlayerComponentInterface 참조 확인
	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		UTA_CombatComponent* CombatComponent = ComponentInterface->GetCombatComponent();

		// 현재 장착된 무기가 있는지 확인
		if (CombatComponent && CombatComponent->CurrentWeapon)
		{
			// 현재 장착된 무기의 Attack 함수 호출
			CombatComponent->CurrentWeapon->Attack();
			UE_LOG(LogTemp, Warning, TEXT("Executed Attack!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No weapon equipped!"));
		}
	}
}

void UTA_InputComponent::InteractionPositive()
{
	UTA_InteractionComponent* InteractionComponent = OwnerPlayer->FindComponentByClass<UTA_InteractionComponent>();
	if(InteractionComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionComponent is null!"));
		return;
	}
	InteractionComponent->InteractionPositive();
}

void UTA_InputComponent::OnRollMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// change state(temp state)
	ChangeState(TempState);

	// temp state == dash
	if (TempState == EPlayerState::PS_Dash)
	{
		// set stamina decrease (true)
		if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
		{
			ComponentInterface->GetCombatComponent()->SetUseStamina(true);
		}
	}
}

void UTA_InputComponent::ChangeState(EPlayerState NewState)
{
	if (PlayerState == NewState) return;

	// switch by Newstate
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
	case EPlayerState::PS_Roll:
		break;
	}

	PlayerState = NewState;
}

