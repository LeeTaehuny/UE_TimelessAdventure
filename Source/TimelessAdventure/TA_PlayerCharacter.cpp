﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_PlayerCharacter.h"
#include "TA_InputComponent.h"
#include "TA_CombatComponent.h"
#include "TA_InteractionComponent.h"
#include "TA_WeaponComponent.h"
#include "TA_WeaponComponent_bow.h"
#include "TA_WeaponComponent_torch.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/TA_InventoryComponent.h"

ATA_PlayerCharacter::ATA_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Survivor_girl_V01/Meshes/SK_Survivor_girl_Cloth_V01.SK_Survivor_girl_Cloth_V01'"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	SpringArmComp->bUsePawnControlRotation = true;

	TA_InputComponent = CreateDefaultSubobject<UTA_InputComponent>(TEXT("InputComponent"));
	TA_CombatComponent = CreateDefaultSubobject<UTA_CombatComponent>(TEXT("TA_CombatComponent"));
	TA_WeaponComponent = CreateDefaultSubobject<UTA_WeaponComponent>(TEXT("WeaponComponent"));
	TA_WeaponComponent_Sword = CreateDefaultSubobject<UTA_WeaponComponent_sword>(TEXT("WeaponComponent_Sword"));
	TA_WeaponComponent_Bow = CreateDefaultSubobject<UTA_WeaponComponent_bow>(TEXT("WeaponComponent_Bow"));
	TA_WeaponComponent_Torch = CreateDefaultSubobject<UTA_WeaponComponent_torch>(TEXT("WeaponComponent_Torch"));
	TA_InteractionComponent = CreateDefaultSubobject<UTA_InteractionComponent>(TEXT("InteractionComponent"));
	DefaultInventory = CreateDefaultSubobject<UTA_InventoryComponent>(TEXT("DefaultInventory"));
	DefaultInventory->MaxInventorySize = 30;


}

void ATA_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(TA_CombatComponent && TA_WeaponComponent)
	{
		TA_CombatComponent->EquipWeapon(TA_WeaponComponent);
		UE_LOG(LogTemp, Warning, TEXT("Fist has been equipped as default weapon"));
	}
}

void ATA_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATA_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TA_InputComponent->AddInput(PlayerInputComponent);

}

void ATA_PlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

