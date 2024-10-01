// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TA_ThrowStone.h"
#include "Interface/MonsterInterface.h"

#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

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
	SetLifeSpan(3.0f);
}

void ATA_ThrowStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	// TODO : ���� ����Ʈ ����
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, GetActorTransform());
	
	// ����
	Destroy();
}

