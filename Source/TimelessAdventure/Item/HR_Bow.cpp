// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_Bow.h"

#include "HR_Arrow.h"


// Sets default values
AHR_Bow::AHR_Bow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component 생성
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Bow SkeletalMesh");
	SetRootComponent(SkeletalMeshComp);
	
	
}

// Called when the game starts or when spawned
void AHR_Bow::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AHR_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHR_Bow::ChangeBowState(EBowState newBowState)
{
	BowState = newBowState;
}

void AHR_Bow::SpawnArrow(USkeletalMeshComponent* Mesh, FName SocketName)
{
	if (ArrowClass)
	{
		AHR_Arrow* SpawnArrow = GetWorld()->SpawnActor<AHR_Arrow>(ArrowClass);
		ArrowIns = SpawnArrow;
		if (ArrowIns)
		{
			ArrowIns->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		}
	}
}

void AHR_Bow::DestroyArrow()
{
	if(ArrowIns)
	{
		ArrowIns->Destroy();
	}
}

void AHR_Bow::FireArrow(FVector Direction)
{
	// Aim 하는 중이면 발사
	if(BowState == EBowState::BS_Aim)
	{
		// Arrow Dispatch
		if(ArrowIns)
		{
			// FDetachmentTransformRules :: Detach 했을 때의 좌표를 어떻게 할지에 대한 설정인듯
			// KeepRelativeTransform -> 원래 좌표가 (0,0,0)이었을 때, 이 좌표를 (0, 0, 0) 유지 => 기존에는 상대 좌표였으므로 좌표 위치가 변함
			// KeepWorldTransform -> 기존 좌표에 맞춰서 월드 좌표로 변환 => 좌표 값은 변경, 위치는 변경 X
			ArrowIns->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			FVector Dir = Direction - ArrowIns->GetActorLocation();
			Dir.Normalize();
			
			// 날라가고자 하는 방향으로 발사
			ArrowIns->Fire(Dir);
		}
	}
}


