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
	bWantsInitializeComponent = true;

	// 멤버 변수 초기화
	MaxStamina = 10.0f;
	CurrentStamina = 0.0f;
	MaxHp = 10.0f;
	CurrrentHp = 0.0f;
	bUseHealth = false;
	UseHealthPercent = 0.001f;	
	bIsAttacking = false;
	ComboCount = 0;
	AttackMoveForce = 1000.0f;
}

void UTA_CombatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerPlayer = Cast<ATA_PlayerCharacter>(GetOwner());
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
}

void UTA_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 지속 체력 증가/감소 여부에 따라 업데이트
	UpdateHealth(bUseHealth, UseHealthPercent);

	// TEST : 로그
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("CurHealth : %.1f"), CurrentStamina));
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, FString::Printf(TEXT("Rate      : %.1f"), CurrentStamina / MaxStamina));
}

void UTA_CombatComponent::Attack()
{
	if (!OwnerPlayer) return;

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

void UTA_CombatComponent::AttackMove()
{
	if (!OwnerPlayer) return;

	// 이동할 방향 + 힘 지정
	FVector Impulse = OwnerPlayer->GetActorForwardVector() * AttackMoveForce;

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

	// 플레이어 상태 전투로 변경
	OwnerPlayer->GetInputComponent()->ChangeState(EPlayerState::PS_Combat);

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

	// 플레이어 상태 일반으로 변경
	OwnerPlayer->GetInputComponent()->ChangeState(EPlayerState::PS_Walk);
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

void UTA_CombatComponent::SetUseHealth(bool Value)
{
	bUseHealth = Value;
}

void UTA_CombatComponent::UpdateHealth(bool Value, float Percent)
{
	// 체력을 사용하지 않고, 현재 체력이 최대 체력보다 낮은 경우
	if (!Value && CurrentStamina < MaxStamina)
	{
		// 전달받은 퍼센트만큼 체력 회복
		CurrentStamina += (Percent * MaxStamina);

		// 최대 체력 넘어간 경우 최대 체력으로 설정
		if (CurrentStamina >= MaxStamina)
		{
			CurrentStamina = MaxStamina;
		}
	}
	// 체력을 사용하고, 현재 체력이 0보다 큰 경우
	else if (Value && CurrentStamina > 0)
	{
		// 전달받은 퍼센트만큼 체력 감소
		CurrentStamina -= (Percent * MaxStamina);

		// 체력이 0보다 작아진 경우
		if (CurrentStamina <= 0.0f)
		{
			// 체력을 0으로 설정
			CurrentStamina = 0.0f;
			// 델리게이트에 연결된 함수 호출
			ZeroHealthDelegate.Broadcast();
			// 지속 체력 증가 설정
			bUseHealth = false;
		}
	}
}

