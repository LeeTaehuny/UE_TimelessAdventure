// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_CombatComponent.h"
#include "Player/TA_PlayerCharacter.h"

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
	bIsAttacking = false;
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

	// 애님 인스턴스 받아오기
	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 공격중이 아닌 경우
	if (!bIsAttacking)
	{
		// 공중에 떠있는 경우
		if (OwnerPlayer->GetCharacterMovement()->IsFalling())
		{
			// 공격 처리
			bIsAttacking = true;

			// 점프 공격 몽타주 재생
			AnimInstance->Montage_Play(JumpAttackMontage);

			FOnMontageEnded MontageEndDelegate;
			MontageEndDelegate.BindUObject(this, &UTA_CombatComponent::JumpAttackMontageEnded);

			AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, JumpAttackMontage);
		}
		// 공중에 떠있지 않은 경우
		else
		{

		}
	}
}

void UTA_CombatComponent::JumpAttackMontageEnded(UAnimMontage* Montage, bool IsEnded)
{
	FTimerHandle EndAttackTimerHandle;
	bIsAttacking = false;
	//GetWorld()->GetTimerManager().SetTimer(EndAttackTimerHandle, FTimerDelegate::CreateLambda([this](){ this->bIsAttacking = false; }), 0.2f, false);
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

