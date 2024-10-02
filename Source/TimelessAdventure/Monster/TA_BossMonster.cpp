﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/TA_BossMonster.h"
#include "Monster/TA_BossController.h"
#include "Monster/TA_AIKeys.h"
#include "Gimmick/TA_ThrowStone.h"
#include "Data/DT_Knockback.h"

#include "MotionWarpingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ATA_BossMonster::ATA_BossMonster()
{
 	PrimaryActorTick.bCanEverTick = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	// AI Controller Setting
	AIControllerClass = ATA_BossController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// AI State Setting
	ChangeState(EBossState::BS_Idle);

	MaxHp = 100.0f;
	CurrentHp = MaxHp;
	Damage = 10.0f;
	AttackDistance = 300.0f;

	bCanJumpBack = false;
}

void ATA_BossMonster::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHp = MaxHp;
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
		TempStone->SetOwner(this);
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

void ATA_BossMonster::BaseAttackCheck()
{
	// 공격 판정 체크
	TArray<FHitResult> HitResults;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * AttackDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(100.0f),
		Params
	);

	if (HitResults.Num() > 0)
	{
		for (const FHitResult& Target : HitResults)
		{
			// 데미지 전달
			UGameplayStatics::ApplyDamage(Target.GetActor(), Damage, GetController(), this, UDamageType::StaticClass());
		}

		DrawDebugSphere(GetWorld(), End, 100.0f, 12, FColor::Red, false, 2.0f);
	}
}

void ATA_BossMonster::KnockbackAttackCheck()
{
	// 공격 판정 체크
	TArray<FHitResult> HitResults;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * AttackDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(100.0f),
		Params
	);

	if (HitResults.Num() > 0)
	{
		for (const FHitResult& Target : HitResults)
		{
			// 데미지 전달
			UGameplayStatics::ApplyDamage(Target.GetActor(), Damage, GetController(), this, UDT_Knockback::StaticClass());
		}

		DrawDebugSphere(GetWorld(), End, 100.0f, 12, FColor::Red, false, 2.0f);
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

void ATA_BossMonster::TeleportAttack()
{
	// 이펙트 스폰
	SetActorHiddenInGame(true);

	// 타이머 설정
	FTimerHandle TeleportHandle;
	GetWorld()->GetTimerManager().SetTimer(TeleportHandle, this, &ATA_BossMonster::TeleportCallBack, 0.5f, false);
}

void ATA_BossMonster::TeleportCallBack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		ATA_BossController* BossController = Cast<ATA_BossController>(GetController());
		if (BossController)
		{
			SetActorHiddenInGame(false);

			AActor* TargetActor = Cast<AActor>(BossController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
			if (TargetActor)
			{
				FVector DirToPlayer = (GetActorLocation() - TargetActor->GetActorLocation()).GetSafeNormal();
				FVector Destination = TargetActor->GetActorLocation() + DirToPlayer * 100.0f;

				TeleportTo(Destination, GetActorRotation(), false, false);

				AnimInstance->Montage_Play(TeleportAttackMontage);

				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &ATA_BossMonster::TeleportAttackEnd);

				AnimInstance->Montage_SetEndDelegate(EndDelegate, TeleportAttackMontage);
			}
		}
	}
}

void ATA_BossMonster::TeleportAttackEnd(UAnimMontage* Montage, bool IsEnded)
{
	OnAttackEndDelegate.Execute();
}

void ATA_BossMonster::BaseAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(BaseAttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ATA_BossMonster::BaseAttackEnd);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, BaseAttackMontage);
	}
}

void ATA_BossMonster::BaseAttackEnd(class UAnimMontage* Montage, bool IsEnded)
{
	OnAttackEndDelegate.Execute();
}

void ATA_BossMonster::KnockbackAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(KnockbackAttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ATA_BossMonster::BaseAttackEnd);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, KnockbackAttackMontage);
	}
}

void ATA_BossMonster::KnockbackAttackEnd(UAnimMontage* Montage, bool IsEnded)
{
	OnAttackEndDelegate.Execute();
}

void ATA_BossMonster::JumpBack(float Distance)
{
	// 초기 1회 무시
	if (!bCanJumpBack)
	{
		bCanJumpBack = true;
		OnJumpEndDelegate.Execute();
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		ATA_BossController* BossController = Cast<ATA_BossController>(GetController());
		if (BossController)
		{
			AActor* TargetActor = Cast<AActor>(BossController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
			if (TargetActor)
			{
				// 방향 설정 (플레이어에서 몬스터 방향)
				FVector Direction = (GetActorLocation() - TargetActor->GetActorLocation()).GetSafeNormal();
				// 목적 위치 설정
				FVector Destination = GetActorLocation() + Direction * Distance;
				// 모션 워핑
				MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
					FName("Target"),
					Destination,
					GetActorRotation()
				);

				AnimInstance->Montage_Play(JumpBackMontage);

				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &ATA_BossMonster::JumpBackEnd);

				AnimInstance->Montage_SetEndDelegate(EndDelegate, JumpBackMontage);
			}
		}
	}
}

void ATA_BossMonster::JumpBackEnd(UAnimMontage* Montage, bool IsEnded)
{
	// 완료 태스크
	OnJumpEndDelegate.Execute();
}

void ATA_BossMonster::SetAIAttackDelegate(const FOnAttackEndDelegate& OnAttackEnd)
{
	OnAttackEndDelegate = OnAttackEnd;
}

void ATA_BossMonster::SetAIJumpDelegate(const FOnJumpBackEndDelegate& OnJumpEnd)
{
	OnJumpEndDelegate = OnJumpEnd;
}

void ATA_BossMonster::ChangeState(EBossState NewState)
{
	if (BossState == NewState) return;

	switch (NewState)
	{
	case EBossState::BS_Idle:
		GetCharacterMovement()->MaxWalkSpeed = 300;
		break;
	case EBossState::BS_Attack:
		GetCharacterMovement()->MaxWalkSpeed = 600;
		break;
	case EBossState::BS_Chase:
		break;
	default:
		break;
	}

	BossState = NewState;
}

void ATA_BossMonster::RangedAttack()
{
	if (CurrentHp >= MaxHp * 0.5f)
	{
		// 1페이즈
		JumpAttack();
	}
	else
	{
		// 2페이즈
		int32 AttackIdx = FMath::RandRange(0, 1);

		switch (AttackIdx)
		{
		case 0:
			ThrowStone();
			break;

		case 1:
			TeleportAttack();
			break;
		}
	}
}

void ATA_BossMonster::MeleeAttack()
{
	// 1 페이즈 / 2 페이즈 공용
	int32 AttackIdx = FMath::RandRange(0, 1);

	switch (AttackIdx)
	{
	case 0:
		BaseAttack();
		break;

	case 1:
		KnockbackAttack();
		break;
	}
}
