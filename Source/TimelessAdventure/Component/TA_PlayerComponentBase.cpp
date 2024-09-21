// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_PlayerComponentBase.h"
#include "Player/TA_PlayerCharacter.h"

UTA_PlayerComponentBase::UTA_PlayerComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}


void UTA_PlayerComponentBase::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerPlayer = Cast<ATA_PlayerCharacter>(GetOwner());
}

// Called when the game starts
void UTA_PlayerComponentBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void UTA_PlayerComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

