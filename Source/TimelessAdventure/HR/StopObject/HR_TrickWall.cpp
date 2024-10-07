// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_TrickWall.h"

#include "Player/TA_PlayerCharacter.h"


// Sets default values
AHR_TrickWall::AHR_TrickWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// physic 활성화
	ObjectMesh->SetSimulatePhysics(true);
	ObjectMesh->SetEnableGravity(false);

	// Overlap event
	ObjectMesh->OnComponentBeginOverlap.AddDynamic(this, &AHR_TrickWall::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void AHR_TrickWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHR_TrickWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHR_TrickWall::Move(float DeltaTime)
{
	// Trigger에 닿은 순간부터 이동 가능
	if(!bIsTrigger) return;

	FVector Direction;
	if(bMovingForward)
	{
		Direction = FVector::RightVector;
	}
	else
	{
		Direction = FVector::LeftVector;
	}

	CheckMoveDir();

	ObjectMesh->AddForce(Direction * 1000.f * ObjectMesh->GetMass());
}

void AHR_TrickWall::Stop()
{
	Super::Stop();

	UE_LOG(LogTemp, Warning, TEXT("Stopped"));
	ObjectMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
}


void AHR_TrickWall::CheckMoveDir()
{
	// 방향만 바꾸면 힘이 남아있어서 힘도 0으로 바꿔줘야할듯 
	if(bMovingForward)
	{
		if(GetActorLocation().Y > EndLocationY)
		{
			bMovingForward = false;
			ObjectMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
			ObjectMesh->AddImpulse(FVector::LeftVector * 1000 * ObjectMesh->GetMass());
		}
	}
	else
	{
		if(GetActorLocation().Y < StartLocationY)
		{
			bMovingForward = true;
			ObjectMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
			ObjectMesh->AddImpulse(FVector::RightVector * 1000 * ObjectMesh->GetMass());
		}
	}
	
}

void AHR_TrickWall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	UE_LOG(LogTemp, Warning, TEXT("OtherActor Name : %s"), *OtherActor->GetName());

	// 일단 나중에 구현
	/*ATA_PlayerCharacter* playerCharacter = Cast<ATA_PlayerCharacter>(OtherActor);
	if(playerCharacter)
	{
		FVector PushDir = OtherActor->GetActorLocation() - GetActorLocation();
		PushDir.Normalize();
		UPrimitiveComponent* prim =  Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		prim->AddImpulse(PushDir * ObjectMesh->GetMass() * 10000);
	}*/
}



