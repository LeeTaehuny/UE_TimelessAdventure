// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StopAbilityComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UHR_StopAbilityComponent::UHR_StopAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// player controller에서 Click 이벤트 사용 설정
	// 근데 default가 true인 것 같음..
	APlayerController* pc =  UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->bEnableClickEvents = true;
	pc->bEnableMouseOverEvents = true;
}


// Called when the game starts
void UHR_StopAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHR_StopAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

