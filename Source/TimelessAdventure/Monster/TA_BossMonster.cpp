// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/TA_BossMonster.h"
#include "Monster/TA_BossController.h"
#include "Monster/TA_AIKeys.h"
#include "Gimmick/TA_ThrowStone.h"

#include "MotionWarpingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ATA_BossMonster::ATA_BossMonster()
{
 	PrimaryActorTick.bCanEverTick = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	// AI Controller Setting
	AIControllerClass = ATA_BossController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// AI State Setting
	ChangeState(EBossState::BS_Idle);
}

void ATA_BossMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATA_BossMonster::ThrowStone()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ThrowAttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ATA_BossMonster::ThrowStoneEnd);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, ThrowAttackMontage);
	}
}

void ATA_BossMonster::ThrowStoneEnd(UAnimMontage* Montage, bool IsEnded)
{
	OnAttackEndDelegate.Execute();
}

void ATA_BossMonster::SpawnStone()
{
	FTransform StoneTransform = GetMesh()->GetSocketTransform(StoneSocketName, ERelativeTransformSpace::RTS_World);
	
	TempStone = GetWorld()->SpawnActor<ATA_ThrowStone>(StoneClass, StoneTransform);
	if (TempStone)
	{
		TempStone->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, StoneSocketName);
	}
}

void ATA_BossMonster::Throw()
{
	if (IsValid(TempStone))
	{
		ATA_BossController* BossController = Cast<ATA_BossController>(GetController());
		if (BossController)
		{
			AActor* TargetActor = Cast<AActor>(BossController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
			FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

			TempStone->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			TempStone->Fire(TargetActor, Direction);
			TempStone = nullptr;
		}
	}
}

void ATA_BossMonster::JumpAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		ATA_BossController* BossController = Cast<ATA_BossController>(GetController());
		if (BossController)
		{
			AActor* TargetActor = Cast<AActor>(BossController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
			if (TargetActor)
			{
				FVector Direction = (GetActorLocation() - TargetActor->GetActorLocation()).GetSafeNormal();
				FVector Destination = TargetActor->GetActorLocation() + Direction * 150.0f;
				MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
					FName("Target"),
					Destination,
					GetActorRotation()
				);

				AnimInstance->Montage_Play(JumpAttackMontage);

				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &ATA_BossMonster::JumpAttackEnd);

				AnimInstance->Montage_SetEndDelegate(EndDelegate, JumpAttackMontage);
			}
		}
	}
}

void ATA_BossMonster::JumpAttackEnd(UAnimMontage* Montage, bool IsEnded)
{
	OnAttackEndDelegate.Execute();
}

void ATA_BossMonster::SetAIAttackDelegate(const FOnAttackEndDelegate& OnAttackEnd)
{
	OnAttackEndDelegate = OnAttackEnd;
}

void ATA_BossMonster::ChangeState(EBossState NewState)
{
	if (BossState == NewState) return;

	switch (BossState)
	{
	case EBossState::BS_Idle:
		GetCharacterMovement()->MaxWalkSpeed = 300;
		break;
	case EBossState::BS_Attack:
		GetCharacterMovement()->MaxWalkSpeed = 600;
		break;
	default:
		break;
	}

	BossState = NewState;
}

void ATA_BossMonster::RangedAttack()
{
	ThrowStone();
	//JumpAttack();
}
