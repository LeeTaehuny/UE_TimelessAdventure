// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StopObjectBase.h"

#include "MovieSceneObjectBindingID.h"


// Sets default values
AHR_StopObjectBase::AHR_StopObjectBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stopable Object Mesh"));
	// RootComponent = ObjectMesh;

	// Material 할당
	ConstructorHelpers::FObjectFinder<UMaterialInstance>SelectableMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/HR/Material/MI_SelectableOverlayColor.MI_SelectableOverlayColor'"));
	if(SelectableMat.Succeeded())
	{
		SelectableMI = SelectableMat.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance>ClickableMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/HR/Material/MI_ClickableOverlayColor.MI_ClickableOverlayColor'"));
	if(SelectableMat.Succeeded())
	{
		ClickableMI = ClickableMat.Object;
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
void AHR_StopObjectBase::OnBeginMouse(AActor* TouchedActor)
{
	// Actor의 Material Clickable Color로 변경
	ChangeMaterialToClickable();
}

void AHR_StopObjectBase::OnEndMouse(AActor* TouchedActor)
{
	// Actor의 Material Selectable Color로 변경 
	ChangeMaterialToSelectable();
}

void AHR_StopObjectBase::OnMouseClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	// @ Player character or StopAbilityComp에서의 정지 time을 가지고 와서 정지 시간을 설정해 주면 좋을 듯
	// 고려 사항 > Material 변경(0.2s 정도)
	// 1) bIsStopped 설정
	// 2) Timer 설정

	bIsStopped = true;
	
	// Create Lamda를 통해서 람다 함수를 생성해서 넘겨주는 것과 그냥 [] 넘겨주는 것과 뭐가 다른가
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
	if(ObjectMesh)
	{
		ObjectMesh->SetOverlayMaterial(nullptr);
	}
}









