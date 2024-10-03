// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/TA_BossMonster.h"
#include "Monster/TA_BossController.h"
#include "Monster/TA_AIKeys.h"
#include "Gimmick/TA_ThrowStone.h"
#include "Data/DT_Knockback.h"

#include "MotionWarpingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"


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

	// 돌 던지기 공격 관련 변수 초기화
	ThrowStoneCoolTime = 5.0f;
	bCanThrowStoneAttack = true;
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
		// 스킬 사용 가능 여부 변경
		bCanThrowStoneAttack = false;

		AnimInstance->Montage_Play(ThrowAttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ATA_BossMonster::ThrowStoneEnd);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, ThrowAttackMontage);
	}
}

void ATA_BossMonster::ThrowStoneEnd(UAnimMontage* Montage, bool IsEnded)
{
	OnAttackEndDelegate.Execute();

	// 쿨타임 적용
	FTimerHandle ThrowStoneTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ThrowStoneTimerHandle, FTimerDelegate::CreateLambda(
		[this]()
		{
			bCanThrowStoneAttack = true;
		}
	), ThrowStoneCoolTime, false);
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

float ATA_BossMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ReslutDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Hit(DamageAmount);

	return ReslutDamage;
}

void ATA_BossMonster::Hit(float HitDamage)
{
	CurrentHp -= HitDamage;

	if (CurrentHp <= 0.0f)
	{
		CurrentHp = 0.0f;

		// AI 상태 사망으로 변경
		ATA_BossController* BossController = Cast<ATA_BossController>(GetController());
		if (BossController)
		{
			BossController->GetBlackboardComponent()->SetValueAsEnum(BBKEY_STATE, static_cast<uint8>(EBossState::BS_Die));
		}
	}
}

void ATA_BossMonster::Die()
{
	Destroy();
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
				
				// 콜리전 프리셋 설정
				GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
				
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

	// 콜리전 프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
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

				// 콜리전 프리셋 설정
				GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));

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

	// 콜리전 프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
}

float ATA_BossMonster::GetHealthPercent()
{
	return (CurrentHp / MaxHp);
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
	case EBossState::BS_Special:
		break;
	default:
		break;
	}

	BossState = NewState;
}

void ATA_BossMonster::RangedAttack()
{
	// 체력이 절반 이상 남아있는 경우
	if (GetHealthPercent() >= 0.5f)
	{
		// 점프 근접 공격 수행
		JumpAttack();
		return;
	}
	
	// 체력이 절반 이하인 경우
	// * 돌 던지기 스킬이 가능한 경우
	if (bCanThrowStoneAttack)
	{
		// 돌 던지기 스킬 사용
		ThrowStone();
	}
	// * 순간이동 공격 사용
	else
	{
		TeleportAttack();
	}
}

void ATA_BossMonster::MeleeAttack()
{
	int32 AttackIdx = FMath::RandRange(0, 2);

	switch (AttackIdx)
	{
	case 0:
		BaseAttack();
		break;

	case 1:
		KnockbackAttack();
		break;

	case 2:
		TeleportAttack();
		break;
	}
}
