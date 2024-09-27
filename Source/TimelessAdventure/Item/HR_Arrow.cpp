// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_Arrow.h"

#include "NaniteSceneProxy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AHR_Arrow::AHR_Arrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Weapon StaticMesh"));
	RootComponent = WeaponMesh;
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
	// ?) 생성자가 아닌 곳에서 CreateDefaultofObject<T> 호출하면 안되나?
	ProjectilMovementComp = NewObject<UProjectileMovementComponent>(this, UProjectileMovementComponent::StaticClass());
	if(ProjectilMovementComp)
	{
		// Actor Comp 등록
		ProjectilMovementComp->RegisterComponent();
		// 변수 설정
		ProjectilMovementComp->UpdatedComponent = WeaponMesh;
		ProjectilMovementComp->bRotationFollowsVelocity = true;
		ProjectilMovementComp->ProjectileGravityScale = 0.5f;

		// 속도 설정
		ProjectilMovementComp->Velocity = Direction * 1000;
	}
	

}

