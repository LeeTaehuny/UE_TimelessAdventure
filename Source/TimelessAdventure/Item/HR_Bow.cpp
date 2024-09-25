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

void AHR_Bow::SpawnArrow(USkeletalMeshComponent* Mesh)
{
	if (ArrowClass)
	{
		AHR_Arrow* Arrow = GetWorld()->SpawnActor<AHR_Arrow>(ArrowClass);
		if (Arrow)
		{
			Arrow->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("arrow_socket_r"));
		}
	}
}


