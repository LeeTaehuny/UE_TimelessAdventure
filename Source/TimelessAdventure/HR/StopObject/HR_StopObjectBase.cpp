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
	ConstructorHelpers::FObjectFinder<UMaterial>SelectableMat(TEXT("/Script/Engine.Material'/Game/HR/Material/M_SelectableColor.M_SelectableColor'"));
	if(SelectableMat.Succeeded())
	{
		SelectableMaterial = SelectableMat.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterial>ClickableMat(TEXT("/Script/Engine.Material'/Game/HR/Material/M_ClickableColor.M_ClickableColor'"));
	if(SelectableMat.Succeeded())
	{
		ClickableMaterial = ClickableMat.Object;
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

	// Default Material 저장
	DefaultMaterial = Cast<UMaterial>(ObjectMesh->GetMaterial(0));
	
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
	}), 5.0f, false);

	
}

void AHR_StopObjectBase::ChangeMaterialToSelectable()
{
	if(SelectableMaterial)
	{
		ObjectMesh->SetMaterial(0, SelectableMaterial);
	}
}

void AHR_StopObjectBase::ChangeMaterialToClickable()
{
	if(ClickableMaterial)
	{
		ObjectMesh->SetMaterial(0, ClickableMaterial);
	}
}

void AHR_StopObjectBase::ChangeMaterialToDefault()
{
	if(DefaultMaterial)
	{
		if(ObjectMesh) ObjectMesh->SetMaterial(0, DefaultMaterial);
	}
}







