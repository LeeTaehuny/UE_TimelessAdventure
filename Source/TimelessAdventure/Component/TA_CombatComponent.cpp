// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_CombatComponent.h"

#include "../Player/TA_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Item/HR_Bow.h"
#include "Camera/CameraComponent.h"
#include "TimelessAdventure/Item/HR_Sword.h"
#include "TimelessAdventure/Item/HR_Torch.h"

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
	// 6) player HUB 켜기
	if(EquipedWeapon == EEquipedWeapon::Bow)
	{
		if(OwnerPlayer)
		{
			OwnerPlayer->SetAimingBow(true);
			OwnerPlayer->SetZooming(true);
			OwnerPlayer->GetCharacterMovement()->bOrientRotationToMovement = false;
			OwnerPlayer->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			OwnerPlayer->ShowPlayerHUB();
			// + characterMovement::RotationRate 조정
		}
		if(BowIns)
		{
			BowIns->ChangeBowState(EBowState::BS_Aim);
			BowIns->SpawnArrow(OwnerPlayer->GetMesh(), ArrowSocketName);
		}
	}
	
}
void UTA_CombatComponent::AimingBowEnd()
{
	
	if(EquipedWeapon == EEquipedWeapon::Bow)
	{
		if(OwnerPlayer)
		{
			OwnerPlayer->SetAimingBow(false);
			OwnerPlayer->SetZooming(false);
			OwnerPlayer->GetCharacterMovement()->bOrientRotationToMovement = true;
			OwnerPlayer->GetCharacterMovement()->bUseControllerDesiredRotation = false;
			OwnerPlayer->HidePlayerHUB();
			// + characterMovement::RotationRate 조정 
		}
	}
	if(BowIns)
	{
		// Aim 종료되면 Destroy
		BowIns->ChangeBowState(EBowState::BS_Idle);
		BowIns->DestroyArrow();
	}
}

void UTA_CombatComponent::FireBow()
{
	// Direction 계산
	if(EquipedWeapon == EEquipedWeapon::Bow)
	{
		if(BowIns)
		{
			BowIns->FireArrow(CalculateArrowDirection());
		}
	}
}

// 무기 생성 부착(Equip)
void UTA_CombatComponent::EquipWeapon()
{
	//
	switch (EquipedWeapon)
	{
	case EEquipedWeapon::Bow:
		BowIns = GetWorld()->SpawnActor<AHR_Bow>(Weapon_Bow);
		BowIns->Equip(BowSocketName, OwnerPlayer->GetMesh());
		break;
	case EEquipedWeapon::Sword:
		SwordIns = GetWorld()->SpawnActor<AHR_Sword>(Weapon_Sword);
		SwordIns->Equip(SwordSocketName, OwnerPlayer->GetMesh());
		break;
	case EEquipedWeapon::Torch:
		TorchIns = GetWorld()->SpawnActor<AHR_Torch>(Weapon_Torch);
		TorchIns->Equip(TorchSocketName, OwnerPlayer->GetMesh());
		break;
	default:
		break;
	}

}

FVector UTA_CombatComponent::CalculateArrowDirection()
{
	FVector Direction;
	UCameraComponent* cameraComp = OwnerPlayer->GetComponentByClass<UCameraComponent>();
	Direction = cameraComp->GetForwardVector();


	// LineTrace
	FVector _Start = cameraComp->K2_GetComponentLocation();
	FVector _End = cameraComp->GetForwardVector() * 10000;
	FHitResult _HitOut;
	FCollisionQueryParams _TraceParams;
	bool success = GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_Visibility, _TraceParams);

	DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, true, 1.0f);

	FVector _From;
	FVector _To;
	if(success)
	{
		_To = _HitOut.Location;
	}
	else _To = _HitOut.TraceEnd;
	_From = BowIns->GetActorLocation();

	Direction = _To;
	
	
	return Direction;
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

