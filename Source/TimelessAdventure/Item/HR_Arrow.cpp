// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_Arrow.h"

#include "NaniteSceneProxy.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AHR_Arrow::AHR_Arrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
}

// Called when the game starts or when spawned
void AHR_Arrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHR_Arrow::Fire(FVector Direction)
{
	// Projectile Movement 이용해서 발사
	// Projectil Movement 동적으로 생성
	ProjectilMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	// Actor Comp 등록
	ProjectilMovementComp->RegisterComponent();
	ProjectilMovementComp->UpdatedComponent = StaticMeshComp;
	// 변수 설정 
	ProjectilMovementComp->bRotationFollowsVelocity = true;
	ProjectilMovementComp->ProjectileGravityScale = 0.5f;

}

