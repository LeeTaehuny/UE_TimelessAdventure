// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_PlayerCharacter.h"
#include "../Component/TA_InputComponent.h"
#include "../Component/TA_CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

void ATA_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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

