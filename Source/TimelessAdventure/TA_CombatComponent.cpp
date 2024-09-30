// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_CombatComponent.h"

#include "MovieSceneTracksComponentTypes.h"
#include "TA_InputComponent.h"
#include "TA_Sword.h"
#include "TA_Torch.h"
#include "TA_WeaponComponent.h"
#include "TA_WeaponComponent_bow.h"
#include "TA_WeaponComponent_sword.h"
#include "TA_WeaponComponent_torch.h"
#include "GameFramework/Character.h"

UTA_CombatComponent::UTA_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Variable Initialize
	MaxStamina = 10.0f;
	CurrentStamina = 0.0f;
	MaxHp = 10.0f;
	CurrrentHp = 0.0f;
	bUseStamina = false;
	UseStaminaPercent = 0.001f;


}

void UTA_CombatComponent::Attack()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Attack();  // 현재 장착된 무기 공격 실행
	}
}


void UTA_CombatComponent::OnNotifyReceived(UAnimNotify* Notify)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh();
		if (CharacterMesh)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = OwnerCharacter;

				UTA_WeaponComponent_sword* SwordWeapon = Cast<UTA_WeaponComponent_sword>(CurrentWeapon);
				UTA_WeaponComponent_torch* TorchWeapon = Cast<UTA_WeaponComponent_torch>(CurrentWeapon);
				if(SpawnedWeaponActor)
				{
					// 기존 무기를 소켓에서 분리하고 파괴
					SpawnedWeaponActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					SpawnedWeaponActor->Destroy();
					SpawnedWeaponActor = nullptr;  // 무기 파괴 후 초기화	
				}
					
				if(SwordWeapon)
				{
					
					// 무기를 생성하고 소켓에 부착
					ATA_Sword* SpawnedSword = World->SpawnActor<ATA_Sword>(SwordClass, OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
					if (SpawnedSword)
					{
						FName SocketName = FName("SwordSocket");
						SpawnedSword->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
						UE_LOG(LogTemp, Warning, TEXT("Sword equipped to socket: %s"), *SocketName.ToString());

						// 현재 무기로 설정
						SpawnedWeaponActor = SpawnedSword;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Failed to spawn sword"));
					}
				}
				else if(TorchWeapon)
				{
					
					ATA_Torch* SpawnedTorch = World->SpawnActor<ATA_Torch>(TorchClass, OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
					if (SpawnedTorch)
					{
						FName SocketName = FName("TorchSocket"); // 토치 소켓 이름 사용
						SpawnedTorch->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
						UE_LOG(LogTemp, Warning, TEXT("Torch equipped to socket: %s"), *SocketName.ToString());

						// 현재 무기로 설정
						SpawnedWeaponActor = SpawnedTorch;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Failed to spawn torch"));
					}
				}
				
			}
		}
	}
}


void UTA_CombatComponent::EquipWeapon(UTA_WeaponComponent* NewWeapon)
{
	// 기존에 장착된 무기를 소켓에서 분리하고 파괴
	if (SpawnedWeaponActor)
	{
		

		if (NewWeapon)
		{
			UTA_CombatComponent::CurrentWeapon = NewWeapon;  // 무기 교체

			// 무기가 sword일 경우에만 손 소켓에 장착
			UTA_WeaponComponent_sword* SwordWeapon = Cast<UTA_WeaponComponent_sword>(NewWeapon);
			UTA_WeaponComponent_torch* TorchWeapon = Cast<UTA_WeaponComponent_torch>(NewWeapon);
			UTA_WeaponComponent* Weapon = Cast<UTA_WeaponComponent>(NewWeapon);
			UTA_WeaponComponent_bow* BowWeapon = Cast<UTA_WeaponComponent_bow>(NewWeapon);


			ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
			if (OwnerCharacter && EquipSwordMontage)
			{
				UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
				if (AnimInstance && !AnimInstance->Montage_IsPlaying(EquipSwordMontage))
				{
					// 장착 애니메이션 실행
					AnimInstance->Montage_Play(EquipSwordMontage);
				
						
				}
			}
		}
		
	}
	else if (NewWeapon)
	{
		UTA_CombatComponent::CurrentWeapon = NewWeapon;  // 무기 교체

		// 무기가 sword일 경우에만 손 소켓에 장착
		UTA_WeaponComponent_sword* SwordWeapon = Cast<UTA_WeaponComponent_sword>(NewWeapon);
		UTA_WeaponComponent_torch* TorchWeapon = Cast<UTA_WeaponComponent_torch>(NewWeapon);
		UTA_WeaponComponent_bow* BowWeapon = Cast<UTA_WeaponComponent_bow>(NewWeapon);


		if (!SwordWeapon && !TorchWeapon && !BowWeapon)
		{
			return;
		}
		else
		{
			
			ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
			if (OwnerCharacter && EquipSwordMontage)
			{
				UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
				if (AnimInstance && !AnimInstance->Montage_IsPlaying(EquipSwordMontage))
				{
					// 장착 애니메이션 실행
					AnimInstance->Montage_Play(EquipSwordMontage);
					
							
				}
			}
		}
	}
	


	
}



void UTA_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UTA_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Stamina update
	UpdateStamina(bUseStamina, UseStaminaPercent);

	// Debug
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("CurHealth : %.1f"), CurrentStamina));
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, FString::Printf(TEXT("Rate      : %.1f"), CurrentStamina / MaxStamina));
}

float UTA_CombatComponent::GetStaminaPercent()
{
	if (MaxStamina == 0.0f) return MaxStamina;
	
	return CurrentStamina / MaxStamina;
}

void UTA_CombatComponent::SetUseStamina(bool Value)
{
	bUseStamina = Value;
}

void UTA_CombatComponent::Init()
{
	// Initialize value (Stamina, HP)
	CurrentStamina = MaxStamina;
	CurrrentHp = MaxHp;
}

void UTA_CombatComponent::UpdateStamina(bool Value, float Percent)
{
	// Not use stamina
	if (!Value && CurrentStamina < MaxStamina)
	{
		// Recovery as used percent
		CurrentStamina += (Percent * MaxStamina);

		// Set stamina less than Maxstamina
		if (CurrentStamina >= MaxStamina)
		{
			CurrentStamina = MaxStamina;
		}
	}
	// Use Stamina && Current Stamina > 0
	else if (Value && CurrentStamina > 0)
	{
		// decrease stamina by percent
		CurrentStamina -= (Percent * MaxStamina);

		// Stamina under zero
		if (CurrentStamina <= 0.0f)
		{
			// set Stamina zero
			CurrentStamina = 0.0f;
			// call function with deligate
			ZeroHealthDelegate.Broadcast();
			// set stamina increase
			bUseStamina = false;
		}
	}
}
void UTA_CombatComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && Montage == EquipSwordMontage && !bInterrupted)
	{
		// Montage가 끝난 후 Locomotion 상태로 자연스럽게 전환
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.5f); // 0.5초 블렌드 아웃으로 전환
		UE_LOG(LogTemp, Warning, TEXT("Montage ended, returning to Locomotion"));
	}
}