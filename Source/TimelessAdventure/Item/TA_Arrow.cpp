// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_Arrow.h"
#include "Player/TA_PlayerCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ATA_Arrow::ATA_Arrow()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	ShaftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShaftMesh"));
	ShaftMesh->SetupAttachment(BaseMesh);

	IronMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IronMesh"));
	IronMesh->SetupAttachment(ShaftMesh);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(IronMesh);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 3000.0f;
	ProjectileMovementComp->MaxSpeed = 5000.0f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;

	AttackDamage = 10.0f;
}

void ATA_Arrow::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovementComp->SetActive(false);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATA_Arrow::OnComponentBeginOverlap);
}

void ATA_Arrow::FireArrow(FVector Pos)
{
	SetLifeSpan(3.0f);
	FVector Direction = (Pos - GetActorLocation()).GetSafeNormal();
	ProjectileMovementComp->Velocity = Direction * ProjectileMovementComp->InitialSpeed;
	ProjectileMovementComp->Activate();
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATA_Arrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 임시로 플레이어 그냥 가져다 체크
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) return;

	// 데미지 전달
	UGameplayStatics::ApplyDamage(OtherActor, AttackDamage, GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->GetPawn(), UDamageType::StaticClass());

	GEngine->AddOnScreenDebugMessage(97, 2.0f, FColor::Red, TEXT("ArrowAttack"));
	Destroy();
}

void ATA_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

