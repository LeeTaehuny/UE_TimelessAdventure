// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_CombatComponent.h"

#include "../Player/TA_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Item/HR_Bow.h"

UTA_CombatComponent::UTA_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 멤버 변수 초기화
	MaxHealth = 10.0f;
	CurrentHealth = 0.0f;
	MaxHp = 10.0f;
	CurrrentHp = 0.0f;
	bUseHealth = false;
	UseHealthPercent = 0.001f;
}



void UTA_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}



void UTA_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 지속 체력 증가/감소 여부에 따라 업데이트
	UpdateHealth(bUseHealth, UseHealthPercent);

	// TEST : 로그
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("CurHealth : %.1f"), CurrentHealth));
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, FString::Printf(TEXT("Rate      : %.1f"), CurrentHealth / MaxHealth));
}

float UTA_CombatComponent::GetHealthPercent()
{
	if (MaxHealth == 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1,  1.0f, FColor::Green, FString::Printf(TEXT("Null")));
		return MaxHealth;
	}
		
	// 체력 퍼센트 반환
	return CurrentHealth / MaxHealth;
}

void UTA_CombatComponent::SetUseHealth(bool Value)
{
	bUseHealth = Value;
}

void UTA_CombatComponent::Init()
{
	// 값 초기화 (체력, HP)
	CurrentHealth = MaxHealth;
	CurrrentHp = MaxHp;

	// 
	OwnerPlayer = Cast<ATA_PlayerCharacter>(GetOwner());

}

// Bow Aim 기능
void UTA_CombatComponent::AimingBowStart()
{
	// 나중에 GetHasBow 대신 Enum  or WeaponBase로 확인
	// 1) bisAiming 변셩
	// 2) bIsZooming 변경
	// 3) Camera 고정
	// 4) Bow 상태 변경
	// 5) Arrow Spawn
	if(OwnerPlayer->GetHasBow())
	{
		if(OwnerPlayer)
		{
			OwnerPlayer->SetAimingBow(true);
			OwnerPlayer->SetZooming(true);
			OwnerPlayer->GetCharacterMovement()->bOrientRotationToMovement = false;
			OwnerPlayer->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			// + characterMovement::RotationRate 조정
		}
	}
	if(BowObject)
	{
		BowObject->ChangeBowState(EBowState::BS_Aim);
		BowObject->SpawnArrow(OwnerPlayer->GetMesh());
	}
}
void UTA_CombatComponent::AimingBowEnd()
{
	
	if(OwnerPlayer->GetHasBow())
	{
		if(OwnerPlayer)
		{
			OwnerPlayer->SetAimingBow(false);
			OwnerPlayer->SetZooming(false);
			OwnerPlayer->GetCharacterMovement()->bOrientRotationToMovement = true;
			OwnerPlayer->GetCharacterMovement()->bUseControllerDesiredRotation = false;
			// + characterMovement::RotationRate 조정 
		}
	}
	if(BowObject)
	{
		BowObject->ChangeBowState(EBowState::BS_Idle);
	}
}

// 무기 생성 부착(Equip)
void UTA_CombatComponent::EquipWeapon()
{
	// 
	if(EquipedWeapon == EEquipedWeapon::Bow)
	{
		AHR_Bow* spawnWeapon = GetWorld()->SpawnActor<AHR_Bow>(Weapon_Bow);
		BowObject = spawnWeapon;
        BowObject->Equip(BowSocketName, OwnerPlayer->GetMesh());
	}
	
}

void UTA_CombatComponent::UpdateHealth(bool Value, float Percent)
{
	// 체력을 사용하지 않고, 현재 체력이 최대 체력보다 낮은 경우
	if (!Value && CurrentHealth < MaxHealth)
	{
		// 전달받은 퍼센트만큼 체력 회복
		CurrentHealth += (Percent * MaxHealth);

		// 최대 체력 넘어간 경우 최대 체력으로 설정
		if (CurrentHealth >= MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
	}
	// 체력을 사용하고, 현재 체력이 0보다 큰 경우
	else if (Value && CurrentHealth > 0)
	{
		// 전달받은 퍼센트만큼 체력 감소
		CurrentHealth -= (Percent * MaxHealth);

		// 체력이 0보다 작아진 경우
		if (CurrentHealth <= 0.0f)
		{
			// 체력을 0으로 설정
			CurrentHealth = 0.0f;
			// 델리게이트에 연결된 함수 호출
			ZeroHealthDelegate.Broadcast();
			// 지속 체력 증가 설정
			bUseHealth = false;
		}
	}
}

