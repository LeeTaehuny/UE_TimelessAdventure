// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_WeaponComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
UTA_WeaponComponent::UTA_WeaponComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void UTA_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UTA_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)

{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UTA_WeaponComponent::Attack()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && AttackAnimation)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackAnimation)
		{

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


void UTA_WeaponComponent::StartAttackSequence(UAnimInstance* AnimInstance, ACharacter* OwnerCharacter)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	int32 RandomSectionIndex = FMath::RandRange(1, 2);
	FString SectionName = FString::Printf(TEXT("Attack%d"), RandomSectionIndex);
	UE_LOG(LogTemp, Warning, TEXT("Starting Attack Sequence: Playing %s"), *SectionName);
	AnimInstance->Montage_Play(AttackAnimation);
	UE_LOG(LogTemp, Warning, TEXT("Montage_JumpToSection 호출됨: %s"), *SectionName);
	AnimInstance->Montage_JumpToSection(FName(*SectionName));

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
	});
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackAnimation);
}

void UTA_WeaponComponent::PlayAttackAnimation()
{
	if(AttackAnimation)
	{
		PlayAnimation(AttackAnimation);
	}
}

void UTA_WeaponComponent::PlayAnimation(UAnimMontage* Animation)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && Animation)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackAnimation)
		{
			AnimInstance->Montage_Play(AttackAnimation);
		}
	}
}

// 이동 입력 시 공격 중단 처리
void UTA_WeaponComponent::CancelAttackAndMove()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimInstance->Montage_IsPlaying(AttackAnimation))
		{
			// 공격 애니메이션 중단
			AnimInstance->Montage_Stop(0.2f, AttackAnimation);

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

		}
	}
}
