// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StopObjectManager.h"

#include "ErrorCodes.h"
#include "../HR_StopObjectBase.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"


// Sets default values
AHR_StopObjectManager::AHR_StopObjectManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHR_StopObjectManager::BeginPlay()
{
	Super::BeginPlay();

	// level에 배치된 모든 StopObject 가져와서 배열에 저장
	// 1) UGameplayStatics::GetAllActorsOfClass() -> 성능이 더 안 좋음
	// 2) Iterator 사용 > EngineUtils.h 헤더 포함해야함 -> 성능이 더 좋다 함

	// TArray<AActor*> TempActors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHR_StopObjectBase::StaticClass(), TempActors);
	// // Cast
	// for (AActor* Actor : TempActors)
	// {
	// 	AHR_StopObjectBase* TempObject = Cast<AHR_StopObjectBase>(Actor);
	// 	if(TempObject)
	// 	{
	// 		StopObjects.Add(TempObject);
	// 	}
	// }

	// 모든 Stop Object 가져오기
	for(TActorIterator<AHR_StopObjectBase> Iter(GetWorld()); Iter; ++Iter)
	{
		AHR_StopObjectBase* TempObject = Cast<AHR_StopObjectBase>(*Iter);
		if(TempObject)
		{
			StopObjects.Add(TempObject);
		}
	}
	
	
}

// Called every frame
void AHR_StopObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHR_StopObjectManager::ChangeMaterialToSelectableAll()
{
	
	// 모든 StopObject Material 변경
	for(AHR_StopObjectBase* StopObject : StopObjects)
	{
		if(StopObject)
		{
			StopObject->ChangeMaterialToSelectable();
		}
	}
}

void AHR_StopObjectManager::ChangeMaterialToDefaultAll()
{
	// 모든 StopObject Material 변경
	for(AHR_StopObjectBase* StopObject : StopObjects)
	{
		if(StopObject)
		{
			StopObject->ChangeMaterialToDefault();
		}
	}
}

