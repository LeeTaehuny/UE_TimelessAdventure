// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_CombatComponent.h"
#include "Player/TA_PlayerCharacter.h"
#include "Component/TA_InputComponent.h"
#include "Data/TA_ComboAttackData.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UTA_CombatComponent::UTA_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 멤버 변수 초기화
	MaxStamina = 10.0f;
	CurrentStamina = 0.0f;
	MaxHp = 10.0f;
	CurrrentHp = 0.0f;
	UseHealthPercent = 0.001f;	
	bIsAttacking = false;
	ComboCount = 0;

	WalkSpeed = 300.0f;
	DashSpeed = 600.0f;
	RollHealthPercent = 0.2f;
}

void UTA_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UTA_CombatComponent::Init()
{
	// 값 초기화 (체력, HP)
	CurrentStamina = MaxStamina;
	CurrrentHp = MaxHp;

	// 이동속도 초기화
	OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void UTA_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 지속 체력 증가/감소 여부에 따라 업데이트
	//UpdateHealth(bUseHealth, UseHealthPercent);

	// 현재 Dash 상태인 경우
	if (CombatState == ECombatState::CS_Dash)
	{
		// 지속 스테미너 감소
		CurrentStamina -= DeltaTime;
		if (CurrentStamina < 0.0f)
		{
			CurrentStamina = 0.0f;
			// Idle 상태로 변경
			ChangeState(ECombatState::CS_Idle);
		}
	}
	// 현재 Idle상태인 경우
	else if (CombatState == ECombatState::CS_Idle)
	{
		// 지속 스테미너 회복
		CurrentStamina += DeltaTime;
		if (CurrentStamina >= MaxStamina)
		{
			CurrentStamina = MaxStamina;
		}
	}

	// TEST : 로그
	{
		FString Name;

		switch (CombatState)
		{
		case ECombatState::CS_Idle:
			Name = TEXT("Idle");
			break;
		case ECombatState::CS_Dash:
			Name = TEXT("Dash");
			break;
		case ECombatState::CS_Roll:
			Name = TEXT("Roll");
			break;
		case ECombatState::CS_Attack:
			Name = TEXT("Attack");
			break;
		default:
			break;
		}

		GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("CurState : %s"), *Name));
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("CurHealth : %.1f"), CurrentStamina));
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, FString::Printf(TEXT("Rate      : %.1f"), CurrentStamina / MaxStamina));
	}
	
}

void UTA_CombatComponent::Walk(FVector ForwardDir, FVector RightDir, FVector2D MovementVector2D)
{
	if (!IsValid(OwnerPlayer)) return;
	if (CombatState == ECombatState::CS_Attack) return;

	OwnerPlayer->AddMovementInput(ForwardDir, MovementVector2D.X);
	OwnerPlayer->AddMovementInput(RightDir, MovementVector2D.Y);
}

void UTA_CombatComponent::DashStart()
{
	if (!IsValid(OwnerPlayer)) return;
	// 플레이어가 Roll/Attack 상태인 경우
	if (CombatState == ECombatState::CS_Roll || CombatState == ECombatState::CS_Attack)
	{
		// 데쉬 상태 임시 저장 후 반환
		TempState = ECombatState::CS_Dash;
		return;
	}

	// 플레이어의 상태가 Idle이 아닌 경우 Dash 불가
	if (CombatState != ECombatState::CS_Idle) return;

	// 상태 변경 (Dush)
	ChangeState(ECombatState::CS_Dash);
}

void UTA_CombatComponent::DashEnd()
{
	if (!IsValid(OwnerPlayer)) return;
	
	// 현재 상태가 Dash인 경우
	if (CombatState == ECombatState::CS_Dash)
	{
		// 상태 변경 (Idle)
		ChangeState(ECombatState::CS_Idle);
		return;
	}

	// 현재 상태가 Roll/Attack인 경우
	if (CombatState == ECombatState::CS_Roll || CombatState == ECombatState::CS_Attack)
	{
		// 기본 상태 임시 저장 후 반환
		TempState = ECombatState::CS_Idle;
		return;
	}
}

void UTA_CombatComponent::RollStart(FVector2D InMovementVector)
{
	if (!IsValid(OwnerPlayer)) return;
	if (CombatState == ECombatState::CS_Roll) return;
	if (OwnerPlayer->GetCharacterMovement()->IsFalling()) return;

	// 현재 체력에서 구르기가 가능한 경우
	if (GetHealthPercent() > RollHealthPercent)
	{
		// 체력 사용
		UseHealth(RollHealthPercent);

		// 임시 상태 저장
		if (CombatState == ECombatState::CS_Idle || CombatState == ECombatState::CS_Dash)
		{
			TempState = CombatState;
		}

		// 상태 변경
		ChangeState(ECombatState::CS_Roll);

		// 임시 저장된 상태에 따라 재생 계수 설정 (걷기 : 1, 달리기 : 1.3)
		float Mult = (TempState == ECombatState::CS_Dash) ? 1.3f : 1.0f;

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
			FRotator TargetRot = (ForwardDirection * InMovementVector.X + RightDirection * InMovementVector.Y).Rotation();

			// 마지막으로 입력된 방향에 따라 즉시 회전
			OwnerPlayer->SetActorRotation(TargetRot);

			// 구르기 몽타주 재생
			AnimInstance->Montage_Play(RollMontage, Mult);

			// 구르기 몽타주 종료 시 호출될 함수 바인딩
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UTA_CombatComponent::RollEnd);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, RollMontage);
		}
	}
}

void UTA_CombatComponent::RollEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// 임시 저장된 상태로 상태 변경
	ChangeState(TempState);
}

void UTA_CombatComponent::CombatJump()
{
	if (!OwnerPlayer) return;
	if (CombatState == ECombatState::CS_Roll) return;

	OwnerPlayer->Jump();
}

void UTA_CombatComponent::Attack()
{
	if (!OwnerPlayer) return;
	// 구르기 상태인 경우 반환
	if (CombatState == ECombatState::CS_Roll) return;

	// 공격중이 아닌 경우
	if (!bIsAttacking)
	{
		// 공중에 떠있는 경우
		if (OwnerPlayer->GetCharacterMovement()->IsFalling())
		{
			JumpAttack();
		}
		// 공중에 떠있지 않은 경우
		else
		{
			ComboStart();
			bIsAttacking = true;
		}
	}
	// 공격 중인 경우 (콤보 타이머가 정상적으로 작동하는 경우)
	else if (ComboTimerHandle.IsValid())
	{
		// 콤보 입력이 들어왔다고 설정
		bIsComboInput = true;
	}
	else
	{
		bIsComboInput = false;
	}
}

void UTA_CombatComponent::AttackMove(float InAttackMoveForce)
{
	if (!OwnerPlayer) return;

	// 이동할 방향 + 힘 지정
	FVector Impulse = OwnerPlayer->GetActorForwardVector() * InAttackMoveForce;

	OwnerPlayer->GetCharacterMovement()->AddImpulse(Impulse, true);
}

void UTA_CombatComponent::JumpAttack()
{
	// 애님 인스턴스 받아오기
	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 공격 처리
	bIsAttacking = true;

	// 점프 공격 몽타주 재생
	AnimInstance->Montage_Play(JumpAttackMontage);

	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UTA_CombatComponent::JumpAttackMontageEnded);

	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, JumpAttackMontage);
}

void UTA_CombatComponent::JumpAttackMontageEnded(UAnimMontage* Montage, bool IsEnded)
{
	bIsAttacking = false;
}

void UTA_CombatComponent::ComboStart()
{
	if (!ComboAttackData) return;

	// 임시 상태 저장
	if (CombatState == ECombatState::CS_Idle || CombatState == ECombatState::CS_Dash)
	{
		TempState = CombatState;
	}

	ChangeState(ECombatState::CS_Attack);

	// 현재 콤보 수 1로 설정
	ComboCount = 1;

	// 애님 인스턴스 받아오기
	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 콤보 몽타주 재생
	AnimInstance->Montage_Play(ComboAttackData->ComboMontage);

	// 몽타주 종료 이벤트 바인딩
	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UTA_CombatComponent::EndCombo);

	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, ComboAttackData->ComboMontage);
	
	// 타이머 핸들 초기화
	ComboTimerHandle.Invalidate();
	// 타이머 설정
	SetComboTimer();
}

void UTA_CombatComponent::EndCombo(UAnimMontage* Montage, bool IsEnded)
{
	// 콤보 관련 변수 초기화
	bIsAttacking = false;
	bIsComboInput = false;
	ComboCount = 0;

	if (CombatState == ECombatState::CS_Attack)
		ChangeState(TempState);
}

void UTA_CombatComponent::SetComboTimer()
{
	// 콤보 인덱스 저장
	// * 콤보 카운트 : 1, 2, 3 ...
	// * 콤보 인덱스 : 0, 1, 2 ...
	int32 ComboIndex = ComboCount - 1;

	// 콤보 인덱스의 콤보 체크 타이머 설정
	// * 체크 인덱스가 유효한 경우
	if (ComboAttackData && ComboAttackData->ComboFrame.IsValidIndex(ComboIndex))
	{
		// 콤보 체크 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UTA_CombatComponent::CheckCombo, ComboAttackData->ComboFrame[ComboIndex], false);
	}
}

void UTA_CombatComponent::CheckCombo()
{
	// 타이머 초기화
	ComboTimerHandle.Invalidate();

	// 콤보 입력이 들어온 경우
	if (bIsComboInput)
	{
		// 콤보 수 증가
		ComboCount++;

		// 콤보 수가 최대 콤보 수를 넘지 않은 경우
		if (ComboAttackData->MaxCount >= ComboCount)
		{
			// 애님 인스턴스 받아오기
			UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
			if (!AnimInstance) return;

			// 콤보 섹션 이름 저장
			FName ComboSectionName = *FString::Printf(TEXT("%s%d"), *ComboAttackData->SectionBaseName, ComboCount);
			// 몽타주 섹션 이동
			AnimInstance->Montage_JumpToSection(ComboSectionName, ComboAttackData->ComboMontage);

			// 타이머 설정
			SetComboTimer();
			// 콤보 입력 초기화
			bIsComboInput = false;
		}
	}
}

float UTA_CombatComponent::GetHealthPercent()
{
	if (MaxStamina == 0.0f) return MaxStamina;

	// 체력 퍼센트 반환
	return CurrentStamina / MaxStamina;
}

void UTA_CombatComponent::UseHealth(float InValue)
{
	// 스테미너 감소
	CurrentStamina -= (InValue * MaxStamina);

	if (CurrentStamina <= 0.0f)
	{
		CurrentStamina = 0.0f;
		// Idle 상태로 변경
		ChangeState(ECombatState::CS_Idle);
	}
}

void UTA_CombatComponent::ChangeState(ECombatState NewState)
{
	if (CombatState == NewState) return;

	switch (NewState)
	{
	case ECombatState::CS_Idle:
		OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;

	case ECombatState::CS_Dash:
		OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
		break;

	case ECombatState::CS_Roll:
		break;

	case ECombatState::CS_Attack:
		break;
	}

	CombatState = NewState;
}