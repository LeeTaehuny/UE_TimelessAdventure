// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StopObjectBase.h"

#include "MovieSceneObjectBindingID.h"
#include "HR/HR_StopAbilityComponent_T.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TA_PlayerCharacter.h"


// Sets default values
AHR_StopObjectBase::AHR_StopObjectBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stopable Object Mesh"));
	// RootComponent = ObjectMesh;

	// Material 할당
	ConstructorHelpers::FObjectFinder<UMaterialInterface>SelectableMat(TEXT("/Script/Engine.Material'/Game/HR/Material/M_SelectableOverlayClor.M_SelectableOverlayClor'"));
	if(SelectableMat.Succeeded())
	{
		SelectableMI = SelectableMat.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface>ClickableMat(TEXT("/Script/Engine.Material'/Game/HR/Material/M_ClickableOverlayColor.M_ClickableOverlayColor'"));
	if(SelectableMat.Succeeded())
	{
		ClickableMI = ClickableMat.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface>StopMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/HR/Material/MI_ClickableOverlayColor.MI_ClickableOverlayColor'"));
	if(StopMat.Succeeded())
	{
		StopMI = StopMat.Object;
	}
	
	// 델리게이트와 바인드
	OnBeginCursorOver.AddDynamic(this, &AHR_StopObjectBase::OnBeginMouse);
	OnEndCursorOver.AddDynamic(this, &AHR_StopObjectBase::OnEndMouse);
	OnClicked.AddDynamic(this, &AHR_StopObjectBase::OnMouseClicked);
}

// Called when the game starts or when spawned
void AHR_StopObjectBase::BeginPlay()
{
	Super::BeginPlay();

	ATA_PlayerCharacter* Player = Cast<ATA_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Player)
	{
		if(IsValid(Player->GetStopAbilityComponent()))
		{
			StopAbilityComponent = Player->GetStopAbilityComponent();
		}
	}
	
}

// Called every frame
void AHR_StopObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsStopped)
	{
		Move(DeltaTime);
	}
}

// 바인딩 함수
// StopAbility의 Detect collider의 안에 들어와야지 동작해야함 
void AHR_StopObjectBase::OnBeginMouse(AActor* TouchedActor)
{
	if(!bIsDetected) return;
	
	// Actor의 Material Clickable Color로 변경
	ChangeMaterialToClickable();
}

void AHR_StopObjectBase::OnEndMouse(AActor* TouchedActor)
{
	if(!bIsDetected) return;
	
	// Actor의 Material Selectable Color로 변경 
	ChangeMaterialToSelectable();
}

void AHR_StopObjectBase::OnMouseClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	Stop();
}

void AHR_StopObjectBase::Stop()
{
	if(!bIsDetected) return;
	
	// @ Player character or StopAbilityComp에서의 정지 time을 가지고 와서 정지 시간을 설정해 주면 좋을 듯
	// 1) bIsStopped 설정
	// 2) Timer 설정
	// 3) 색도 변경
	// 4) 시간 게이지 사용
	
	bIsStopped = true;
	ChangeMaterialToStop();
	StopAbilityComponent->UseTimeEnergy();
	StopAbilityComponent->StopAbilityEnd();
	
	
	// ? Create Lamda를 통해서 람다 함수를 생성해서 넘겨주는 것과 그냥 [] 넘겨주는 것과 뭐가 다른가
	GetWorld()->GetTimerManager().SetTimer(StopTimer, FTimerDelegate::CreateLambda([this](){
		bIsStopped = false;
		ChangeMaterialToDefault();
	}), 5.0f, false);
}

void AHR_StopObjectBase::ChangeMaterialToSelectable()
{
	if(SelectableMI && ObjectMesh)
	{
		ObjectMesh->SetOverlayMaterial(SelectableMI);
	}
}

void AHR_StopObjectBase::ChangeMaterialToClickable()
{
	if(SelectableMI && ObjectMesh)
	{
		ObjectMesh->SetOverlayMaterial(ClickableMI);
	}
}

void AHR_StopObjectBase::ChangeMaterialToDefault()
{
	if(ObjectMesh &&!bIsStopped)
	{
		ObjectMesh->SetOverlayMaterial(nullptr);
	}
}

void AHR_StopObjectBase::ChangeMaterialToStop()
{
	if(StopMI && ObjectMesh)
	{
		ObjectMesh->SetOverlayMaterial(StopMI);
	}
}









