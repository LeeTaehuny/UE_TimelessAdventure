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
	// 타겟 설정
	ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
	
	// 방향 및 속도 지정
	ProjectileMovementComponent->Velocity = Direction * InitSpeed;

	// 발사
	ProjectileMovementComponent->SetActive(true);
}

void ATA_ThrowStone::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 맞은 액터가 캐릭터인 경우
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		// 몬스터 인터페이스 받아오기
		if (IMonsterInterface* MI = Cast<IMonsterInterface>(GetOwner()))
		{
			// 데미지 전달
			UGameplayStatics::ApplyDamage(Character, MI->GetDamage(), GetOwner()->GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
		}
	}

	// TODO : 폭발 이펙트 생성
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, GetActorTransform());
	
	// 삭제
	Destroy();
}

