// Fill out your copyright notice in the Description page of Project Settings.



#include "TA_WeaponComponent_sword.h"

#include "IPersonaPreviewScene.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UTA_WeaponComponent_sword::UTA_WeaponComponent_sword()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentComboIndex = -1;
	QueuedAttacks = 0;
	bIsAttacking = false;

}

void UTA_WeaponComponent_sword::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerPlayer = Cast<ACharacter>(GetOwner());
}

void UTA_WeaponComponent_sword::Attack()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && AttackAnimation)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackAnimation)
		{

			if(AnimInstance->Montage_IsPlaying(AttackAnimation))
			{
				QueuedAttacks++;
				return;
			}
			QueuedAttacks = 0;
			StartAttackSequence(AnimInstance, OwnerCharacter);

			// 공격 중 캐릭터 회전 잠금
			OwnerCharacter->bUseControllerRotationYaw = false; // 캐릭터 회전 잠금
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false; // 이동에 따른 자동 회전 방지
			APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
			if(PlayerController)
			{
				PlayerController->SetIgnoreMoveInput(true);  // 이동 입력 무시
				PlayerController->SetIgnoreLookInput(true);
			}
		}
	}
}

void UTA_WeaponComponent_sword::StartAttackSequence(UAnimInstance* AnimInstance, ACharacter* OwnerCharacter)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	int32 RandomSectionIndex = FMath::RandRange(1, 4);
	FString SectionName = FString::Printf(TEXT("Attack%d"), RandomSectionIndex);
	UE_LOG(LogTemp, Warning, TEXT("Starting Attack Sequence: Playing %s"), *SectionName);
	AnimInstance->Montage_Play(AttackAnimation);
	UE_LOG(LogTemp, Warning, TEXT("Montage_JumpToSection 호출됨: %s"), *SectionName);
	AnimInstance->Montage_JumpToSection(FName(*SectionName));

	bIsComboActive = true;
	CurrentComboIndex = RandomSectionIndex;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this, OwnerCharacter](UAnimMontage* Montage, bool bInterrupted)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		OwnerCharacter->bUseControllerRotationYaw = false;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

		APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
		if(PlayerController)
		{
			PlayerController->SetIgnoreMoveInput(false);
			PlayerController->SetIgnoreLookInput(false);
		}
		if(QueuedAttacks > 0)
		{
			QueuedAttacks--;
			StartAttackSequence(OwnerCharacter->GetMesh()->GetAnimInstance(), OwnerCharacter);
		}
		else
		{
			ResetCombo();
			bIsComboActive = false;
			QueuedAttacks = 0;
		}
	});
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackAnimation);
}
void UTA_WeaponComponent_sword::ResetCombo()
{
	CurrentComboIndex = -1;
	QueuedAttacks = 0;
}

void UTA_WeaponComponent_sword::UpdateCombo()
{
	CurrentComboIndex = FMath::RandRange(1, 4);
}



// 이동 입력 시 공격 중단 처리
void UTA_WeaponComponent_sword::CancelAttackAndMove()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimInstance->Montage_IsPlaying(AttackAnimation))
		{
			// 공격 애니메이션 중단
			AnimInstance->Montage_Stop(0.2f, AttackAnimation);
			ResetCombo();

			// 이동 및 회전 복구
			OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			OwnerCharacter->bUseControllerRotationYaw = true;
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

			APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
			if (PlayerController)
			{
				PlayerController->SetIgnoreMoveInput(false);
				PlayerController->SetIgnoreLookInput(false);
			}

			QueuedAttacks = 0;  // 공격 대기열 초기화
		}
	}
}

void UTA_WeaponComponent_sword::OnNotifyReceived(UAnimNotify* Notify)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (Notify && Notify->GetNotifyName().Equals("EndAttackSection"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify Received: EndAttackSection"));
		if (QueuedAttacks > 0)
		{
			QueuedAttacks--;
			StartAttackSequence(OwnerCharacter->GetMesh()->GetAnimInstance(), OwnerCharacter);
		}
		else
		{
			ResetCombo();
			bIsComboActive = false;
		}
	}
}