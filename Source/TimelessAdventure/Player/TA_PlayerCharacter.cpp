// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_PlayerCharacter.h"
#include "../Component/TA_InputComponent.h"
#include "../Component/TA_CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "TimelessAdventure/Item/HR_Bow.h"

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

	// Bow 임시 부착
	// 나중에 Combat Component에서 무기 교환할때 사용
	TA_CombatComponent->EquipWeapon();
	
	

	// Zoom 변수들 초기화
	InitialSO = SpringArmComp->SocketOffset;
	InitialFOV = CameraComp->FieldOfView;
	
}

void ATA_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// bisZooming 변수를 combatComp에서 변경
	if(bisZooming)
	{
		// Camera의 Fiel of view
		CameraComp->FieldOfView = FMath::FInterpTo(CameraComp->FieldOfView, AimFOV, DeltaTime, 5.0f);
		//CameraComp->SetFieldOfView(AimFOV);
		SpringArmComp->SocketOffset.X = FMath::FInterpTo(SpringArmComp->SocketOffset.X, AimSO.X, DeltaTime, 5.0f);
		SpringArmComp->SocketOffset.Y = FMath::FInterpTo(SpringArmComp->SocketOffset.Y, AimSO.Y, DeltaTime, 5.0f);
		SpringArmComp->SocketOffset.Z = FMath::FInterpTo(SpringArmComp->SocketOffset.Z, AimSO.Z, DeltaTime, 5.0f);
		
	}
	else
	{
		CameraComp->FieldOfView = FMath::FInterpTo(CameraComp->FieldOfView, InitialFOV, DeltaTime, 10.0f);
		SpringArmComp->SocketOffset.X = FMath::FInterpTo(SpringArmComp->SocketOffset.X, InitialSO.X, DeltaTime, 10.0f);
		SpringArmComp->SocketOffset.Y = FMath::FInterpTo(SpringArmComp->SocketOffset.Y, InitialSO.Y, DeltaTime, 10.0f);
		SpringArmComp->SocketOffset.Z = FMath::FInterpTo(SpringArmComp->SocketOffset.Z, InitialSO.Z, DeltaTime, 10.0f);
	}
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



