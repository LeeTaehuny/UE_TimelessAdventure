// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_WeaponComponent.h"

#include "GameFramework/Character.h"

// Sets default values
UTA_WeaponComponent::UTA_WeaponComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void UTA_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UTA_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)

{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UTA_WeaponComponent::Attack()
{
	PlayAttackAnimation();
	
}

void UTA_WeaponComponent::PlayAttackAnimation()
{
	if(AttackAnimation)
	{
		PlayAnimation(AttackAnimation);
	}
}

void UTA_WeaponComponent::PlayAnimation(UAnimMontage* Animation)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && Animation)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackAnimation)
		{
			AnimInstance->Montage_Play(AttackAnimation);
		}
	}
}

