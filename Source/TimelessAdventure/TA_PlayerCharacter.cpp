// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"

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
}

void ATA_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && IMC_Player)
	{
		// Call SubSystem
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add Mapping Context
			SubSystem->AddMappingContext(IMC_Player, 0);
			// Input Start
			EnableInput(PlayerController);
		}
	}
}

void ATA_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATA_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATA_PlayerCharacter::BasicLook);
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATA_PlayerCharacter::BasicMove);
}

void ATA_PlayerCharacter::BasicMove(const FInputActionValue& Value)
{
	// value extraction
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Controller rotation Yaw value
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward and right directions based on rotation(Yaw) (Y: forward, X : right)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Add Movement
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ATA_PlayerCharacter::BasicLook(const FInputActionValue& Value)
{
	// value extraction
	FVector2D LookVector = Value.Get<FVector2D>();

	// Add Controller Input
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y * -1.0f);
}

