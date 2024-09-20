// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_PlayerComponentBase.h"

#include "GameFramework/Character.h"

UTA_PlayerComponentBase::UTA_PlayerComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;

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

void UTA_PlayerComponentBase::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerPlayer = Cast<ACharacter>(GetOwner());
}