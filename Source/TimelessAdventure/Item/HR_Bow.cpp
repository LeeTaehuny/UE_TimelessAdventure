// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_Bow.h"

#include "HR_Arrow.h"


// Sets default values
AHR_Bow::AHR_Bow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component 생성
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon SkeletalMesh");
	
	
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

void AHR_Bow::FireArrow()
{
	// Aim 하는 중이면 발사
	if(BowState == EBowState::BS_Aim)
	{
		// Arrow Dispatch
		if(ArrowIns)
		{
			ArrowIns->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			//ArrowIns->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
		// 날라가고자 하는 방향으로 발사
		// 
		FVector Direction = FVector(0, 0, 0); 
		ArrowIns->Fire(Direction);
	}
}


