// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Inventory/NPC.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	SetRootComponent(BoxComp);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FocusCamera"));
	CameraComp->SetupAttachment(RootComponent);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(StaticMesh);

	InteractionWidgetComponent->SetRelativeLocation(FVector(0, 0, 220.0f));
	InteractionWidgetComponent->SetRelativeRotation(FRotator(90,0,0));
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	InteractionWidgetComponent->SetDrawSize(FVector2D(50.0f,50.0f));

	if(InteractionWidgetClass)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
	}
	
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	InteractionWidgetComponent->SetVisibility(false);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::DisplayInteractionWidget()
{
	if(InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(true);
	}
}
void ANPC::HideInteractionWidget()
{
	if(InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(false);
	}
}
void ANPC::Interact()
{
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	//if(OwnerActor && OurPlayerController)
	//{
	OurPlayerController->SetViewTargetWithBlend(this, 1.f);
	OurPlayerController->SetInputMode(FInputModeUIOnly());
	OurPlayerController->SetShowMouseCursor(true);
	HideInteractionWidget();
	//}
	if(ItemShopWidgetClass)
	{
		ItemShopWidget = CreateWidget<UUserWidget>(GetWorld(), ItemShopWidgetClass);

		FName PropertyName(TEXT("OwnerNPC"));
		FProperty* _NPCProperty = ItemShopWidget->GetClass()->FindPropertyByName(PropertyName);
		if(_NPCProperty)
		{
			FObjectProperty* ObjectProperty = CastField<FObjectProperty>(_NPCProperty);
			if(ObjectProperty && ObjectProperty->PropertyClass == ANPC::StaticClass())
			{
				ObjectProperty->SetObjectPropertyValue_InContainer(ItemShopWidget, this);
			}
		}
		if(ItemShopWidget)ItemShopWidget->AddToViewport();

	}
	UE_LOG(LogTemp, Log, TEXT("Interacted with object"));
}