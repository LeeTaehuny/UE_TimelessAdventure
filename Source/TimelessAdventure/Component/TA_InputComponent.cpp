// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_InputComponent.h"
#include "TA_CombatComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Animation/AnimMontage.h"
#include "../Interface/PlayerComponentInterface.h"
#include "../Player/TA_PlayerCharacter.h"


UTA_InputComponent::UTA_InputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	WalkSpeed = 300.0f;
	DashSpeed = 600.0f;
	RollHealthPercent = 0.2f;
}

void UTA_InputComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerPlayer = Cast<ATA_PlayerCharacter>(GetOwner());
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

	// 플레이어의 초기 이동속도는 걷기 속도로 지정
	OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	// ZeroHealthDelegate에 함수 매핑
	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		ComponentInterface->GetCombatComponent()->ZeroHealthDelegate.AddUObject(this, &UTA_InputComponent::DashEnd);
	}

	// Notify
	animInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
	if(animInstance)
	{
		animInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UTA_InputComponent::HandleOnMontageSwordAttackNotifyBegin);
		
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
		EnhancedInputComponent->BindAction(IA_SwordAttack, ETriggerEvent::Started, this, &UTA_InputComponent::SwordAttack);
		EnhancedInputComponent->BindAction(IA_AimBow, ETriggerEvent::Started, this, &UTA_InputComponent::AimBowStart);
		EnhancedInputComponent->BindAction(IA_AimBow, ETriggerEvent::Completed, this, &UTA_InputComponent::AimBowEnd);
	}
}

void UTA_InputComponent::BasicMove(const FInputActionValue& Value)
{
	if (!IsValid(OwnerPlayer)) return;
	// 공격할때 못 움직임
	if(IsAttacking() || isAttack) return;

	MovementVector = Value.Get<FVector2D>();

	// Controller rotation Yaw값 저장
	const FRotator Rotation = OwnerPlayer->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Yaw값을 기준으로 전방과 우측 방향 가져오기 (Y: forward, X : right)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	OwnerPlayer->AddMovementInput(ForwardDirection, MovementVector.X);
	OwnerPlayer->AddMovementInput(RightDirection, MovementVector.Y);
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

	// 플레이어가 구르기 상태인 경우
	if (PlayerState == EPlayerState::PS_Roll)
	{
		// 데쉬 상태 임시 저장후 반환
		TempState = EPlayerState::PS_Dash;
		return;
	}
	// 플레이어의 상태가 Walk가 아닌 경우 Dash 불가
	if (PlayerState != EPlayerState::PS_Walk) return;

	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		// 지속적으로 체력을 사용한다고 설정
		ComponentInterface->GetCombatComponent()->SetUseHealth(true);
	}

	// 상태 변경 (Dush)
	ChangeState(EPlayerState::PS_Dash);
}

void UTA_InputComponent::DashEnd()
{
	if (!IsValid(OwnerPlayer)) return;

	// 플레이어가 구르기 상태인 경우
	if (PlayerState == EPlayerState::PS_Roll)
	{
		// 걷기 상태 임시 저장 후 반환
		TempState = EPlayerState::PS_Walk;
		return;
	}

	// 상태 변경 (Walk)
	ChangeState(EPlayerState::PS_Walk);

	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		// 지속적으로 체력을 사용하지 않겠다고 설정
		ComponentInterface->GetCombatComponent()->SetUseHealth(false);
	}
}

void UTA_InputComponent::BasicRoll()
{
	if (!IsValid(OwnerPlayer)) return;
	if (PlayerState == EPlayerState::PS_Roll) return;
	if (OwnerPlayer->GetCharacterMovement()->IsFalling()) return;

	if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
	{
		// 현재 체력에서 구르기가 가능한 경우
		if (ComponentInterface->GetCombatComponent()->GetHealthPercent() > RollHealthPercent)
		{
			// 체력 즉시 사용
			ComponentInterface->GetCombatComponent()->UpdateHealth(true, RollHealthPercent);

			// 현재 플레이어의 상태(걷기/달리기) 임시 저장
			if (PlayerState == EPlayerState::PS_Walk || PlayerState == EPlayerState::PS_Dash)
			{
				TempState = PlayerState;
			}

			// 상태 변경 (PS_Roll)
			ChangeState(EPlayerState::PS_Roll);

			// 임시 저장된 상태에 따라 재생 계수 설정 (걷기 : 1, 달리기 : 1.3)
			float Mult = TempState == EPlayerState::PS_Dash ? 1.3f : 1.0f;

			UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				// Controller rotation Yaw 값 저장
				const FRotator Rotation = OwnerPlayer->Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// Yaw값을 기준으로 전방과 우측 방향 가져오기 (Y: forward, X : right)
				const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

				// 전방, 우측 방향을 기준으로 회전해야 하는 방향 구하기 (입력 값)
				FRotator TargetRot = (ForwardDirection * MovementVector.X + RightDirection * MovementVector.Y).Rotation();

				// 마지막으로 입력된 방향에 따라 즉시 회전
				OwnerPlayer->SetActorRotation(TargetRot);

				// 구르기 몽타주 재생
				AnimInstance->Montage_Play(RollMontage, Mult);

				// 구르기 몽타주 종료 시 호출될 함수 바인딩
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

	if (IsValid(OwnerPlayer) && !IsAttacking())
	{
		OwnerPlayer->Jump();
	}
}

void UTA_InputComponent::SwordAttack()
{
	if (!IsValid(OwnerPlayer)) return;
	if (OwnerPlayer->GetCharacterMovement()->IsFalling()) return;

	// 활 들고 있을 때는 무시
	ATA_PlayerCharacter* Character = Cast<ATA_PlayerCharacter>(OwnerPlayer);
	UE_LOG(LogTemp, Warning, TEXT("%d"), Character->GetHasBow());
	if(Character->GetHasBow()) return;

	// 구르기 & 점프 중에는 공격 못함
	// 걷기 & 달리기 중에는 걷기 달리기 멈추고 공격 
	if(!IsAttacking() && PlayerState != EPlayerState::PS_Roll)
	{
		animInstance->Montage_Play(SwordComboAttackMontage);
	}
	else
	{
		SwordComboIndex = 1;
	}
	
}

void UTA_InputComponent::AimBowStart()
{

	OwnerPlayer->GetCombatComponent()->AimingBowStart();
	
	// bisAimingbow 체크
	//if(OwnerPlayer) OwnerPlayer->SetAimingBow(true);
	
	// Bow Attack 중에는 이동 못함
	// jump & roll 중에는 공격 못함 
	/*ATA_PlayerCharacter* playerCharacter = Cast<ATA_PlayerCharacter>(OwnerPlayer);
	if(playerCharacter->GetHasBow())
	{
		playerCharacter->SetAimingBow(true);	
	}*/
	//isAttack = true;
}

void UTA_InputComponent::AimBowEnd()
{
	OwnerPlayer->GetCombatComponent()->AimingBowEnd();
	
	// bisAimingbow 체크
	//if(OwnerPlayer) OwnerPlayer->SetAimingBow(false);
	
	/*ATA_PlayerCharacter* playerCharacter = Cast<ATA_PlayerCharacter>(OwnerPlayer);
	if(playerCharacter->GetHasBow())
	{
		playerCharacter->SetAimingBow(false);	
	}*/

	// Animation이 끝났을때 움직일 수 있게
	//isAttack = false;
}


void UTA_InputComponent::HandleOnMontageSwordAttackNotifyBegin(FName notifyName,
                                                               const FBranchingPointNotifyPayload& branchingpayload)
{
	// combo index 감소 + combo index
	SwordComboIndex--;

	if(SwordComboIndex < 0)
	{
		animInstance->Montage_Stop(0.4f, SwordComboAttackMontage);
	}
}

void UTA_InputComponent::OnRollMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// 임시 저장된 상태로 상태 변경
	ChangeState(TempState);

	// 임시 상태가 데쉬 상태인 경우
	if (TempState == EPlayerState::PS_Dash)
	{
		// 체력 감소 설정
		if (IPlayerComponentInterface* ComponentInterface = Cast<IPlayerComponentInterface>(OwnerPlayer))
		{
			ComponentInterface->GetCombatComponent()->SetUseHealth(true);
		}
	}
}

void UTA_InputComponent::ChangeState(EPlayerState NewState)
{
	if (PlayerState == NewState) return;

	// 새로 들어온 상태에 따라 처리
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

bool UTA_InputComponent::IsAttacking()
{
	return animInstance->Montage_IsPlaying(SwordComboAttackMontage);
}

