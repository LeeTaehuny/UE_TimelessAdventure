// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TA_ThrowStone.h"
#include "Interface/MonsterInterface.h"

#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h" 
#include "NiagaraSystem.h" 
#include "HR/HR_StopAbilityComponent_T.h"

ATA_ThrowStone::ATA_ThrowStone()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereComponent);

	InitSpeed = 300.0f;
	MaxSpeed = 300.0f;
	HomingAccelerationMagnitude = 300.0f;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjectileMovementComponent->InitialSpeed = InitSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATA_ThrowStone::OnComponentBeginOverlap);
}

void ATA_ThrowStone::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovementComponent->SetActive(false);
	
	FTimerHandle DeleteTimerHandle;
	FTimerDelegate DeleteDelegate;
	DeleteDelegate.BindUObject(this, &ATA_ThrowStone::DestroyStone);

	GetWorld()->GetTimerManager().SetTimer(DeleteTimerHandle, DeleteDelegate, 3.0f, false);
}

void ATA_ThrowStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATA_ThrowStone::Move(float DeltaTime)
{
	
}

void ATA_ThrowStone::Fire(AActor* Target, FVector Direction)
{
	// Ÿ�� ����
	ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
	
	// ���� �� �ӵ� ����
	ProjectileMovementComponent->Velocity = Direction * InitSpeed;

	// �߻�
	ProjectileMovementComponent->SetActive(true);
}

void ATA_ThrowStone::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("otherComp Name %s"), *OtherComp->GetName());
	if (OtherComp->GetCollisionProfileName() == TEXT("Time"))
	{
		return;
	}
	
	// ���� ���Ͱ� ĳ������ ���
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		// ���� �������̽� �޾ƿ���
		if (IMonsterInterface* MI = Cast<IMonsterInterface>(GetOwner()))
		{
			// ������ ����
			UGameplayStatics::ApplyDamage(Character, MI->GetDamage(), GetOwner()->GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
		}
	}
	
	DestroyStone();
}

void ATA_ThrowStone::DestroyStone()
{
	// ����Ʈ ����
	if (HitFX)
	{
		FXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX, GetActorLocation(), GetActorRotation());
	}

	// ����
	Destroy();
}

// void ATA_ThrowStone::OnMouseClicked(AActor* TouchedActor, FKey ButtonPressed)
// {
// 	// Physics 멈추기
// 	
// 	if(!bIsDetected) return;
// 	
// 	// @ Player character or StopAbilityComp에서의 정지 time을 가지고 와서 정지 시간을 설정해 주면 좋을 듯
// 	// 1) bIsStopped 설정
// 	// 2) Timer 설정
// 	// 3) 색도 변경
// 	// 4) 시간 게이지 사용
// 	
// 	bIsStopped = true;
// 	ChangeMaterialToStop();
// 	StopAbilityComponent->UseTimeEnergy();
// 	StopAbilityComponent->StopAbilityEnd();
//
// 	// physic 설정
// 	ProjectileMovementComponent->SetActive(false);
// 	ProjectileMovementComponent->StopMovementImmediately();
// 	
// 	
// 	// Projectile movement comp 조절 
// 	GetWorld()->GetTimerManager().SetTimer(StopTimer, FTimerDelegate::CreateLambda([this](){
// 		ProjectileMovementComponent->SetActive(true);
// 		ChangeMaterialToDefault();
// 	}), 5.0f, false);
//
// 	
//}

