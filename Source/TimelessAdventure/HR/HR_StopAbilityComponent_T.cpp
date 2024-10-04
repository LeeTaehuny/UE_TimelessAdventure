// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StopAbilityComponent_T.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StopObject/HR_StopObjectBase.h"


// Sets default values for this component's properties
UHR_StopAbilityComponent_T::UHR_StopAbilityComponent_T()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UHR_StopAbilityComponent_T::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	// player controller에서 Click 이벤트 사용 설정
	// 근데 default가 true인 것 같음..
	// ! pc는 생성자에서 받아오면 안됨 -> 생성자 호출 시기에는 pc가 없는듯 
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// 이벤트 비활성화
	DeactivateMouseEvent();
	DetectCollider->SetGenerateOverlapEvents(false);
	
}

void UHR_StopAbilityComponent_T::OnRegister()
{
	Super::OnRegister();

	// Sphere component 등록 
	DetectCollider = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("Detect Collider"));
	DetectCollider->SetSphereRadius(Radius);
	DetectCollider->SetupAttachment(this);
	DetectCollider->RegisterComponent();
	

	// 델리게이트에 바인딩
	DetectCollider->OnComponentBeginOverlap.AddDynamic(this, &UHR_StopAbilityComponent_T::OnComponentBeginOverlap);
	DetectCollider->OnComponentEndOverlap.AddDynamic(this, &UHR_StopAbilityComponent_T::OnComponentEndOverlap);

	// 이벤트 비활성화
	DetectCollider->SetGenerateOverlapEvents(false);

}



// Called every frame
void UHR_StopAbilityComponent_T::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bIsTabClick)
	{
		// Collision 그리기
		DrawDebugSphere(GetWorld(), DetectCollider->GetComponentLocation(), DetectCollider->GetScaledSphereRadius(), 12, FColor::Green, false, DeltaTime, 0, 1.0f);
	}
}


void UHR_StopAbilityComponent_T::StopAbilityBegin()
{
	// Stop Object mode 시작
	// 1) 마우스 커서 보이기
	//		- 마우스 커서 crosshair로 변경
	//		- 마우스 위치 중심으로 초기화ㅁㄴ
	// 2) 마우스 이벤트 Active
	// 3) StopableObject를 관리하는 Manager를 통해서 Stopable Object들의 Material을 수정
	//		> Stopable Object를 TArray를 통해서 모든 Object material 변경
	//		> Component에 Collider를 붙여서 그 범위 안에 있는 Stopable Object들의 Material 변경
	PlayerController->bShowMouseCursor = true;
	PlayerController->CurrentMouseCursor = EMouseCursor::Crosshairs;
	PlayerController->SetMouseLocation(1000, 350);
	ActivateMouseEvent();

	// Line 그리기 위한 bool
	bIsTabClick = true;
	// overlap 이벤트 활성화
	DetectCollider->SetGenerateOverlapEvents(true);

	//StopObjectManager->ChangeMaterialToSelectableAll();
}

// 마우스 클릭시 호출
// overlap 이벤트 비활성화, 마우스 커서 끄기, 마우스 이벤트 비활성화 
// 1) stop object 클릭 -> 색 변경 X
// 2) X 클릭 -> Material 색 Default로 변경

// 마우스 클릭 받음
// over begin >
// over end > 
// click > 

// overlap
// begin -> 머터리얼 Selectable로 수정 
// end -> 머터리얼 Default로 수정 
void UHR_StopAbilityComponent_T::StopAbilityEnd()
{

	// 1) Overlap 이벤트 비활성화
	// 2) 마우스 이벤트 비활성화
	// 3) 마우스 커서 끄기

	// Line 그리기 위한 bool
	bIsTabClick = false;
	
	// overlap 이벤트 비활성화
	DetectCollider->SetGenerateOverlapEvents(false);
	// 커서 끄기	
	PlayerController->bShowMouseCursor = false;
	PlayerController->CurrentMouseCursor = EMouseCursor::Default;
	// 마우스 이벤트 비화성화 
	DeactivateMouseEvent();
	
	
}

void UHR_StopAbilityComponent_T::ActivateMouseEvent()
{
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
}

void UHR_StopAbilityComponent_T::DeactivateMouseEvent()
{
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;
}

void UHR_StopAbilityComponent_T::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// StopObjectBase인경우 Material Selectable로 변경
	// Detected 됐는지 체크
	AHR_StopObjectBase* StopObject = Cast<AHR_StopObjectBase>(OtherActor);
	if(StopObject)
	{
		StopObject->ChangeMaterialToSelectable();
		StopObject->SetbIsDetected(true);
	}
}

void UHR_StopAbilityComponent_T::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// StopObjectBase인경우 Material Default로 변경
	AHR_StopObjectBase* StopObject = Cast<AHR_StopObjectBase>(OtherActor);
	if(StopObject)
	{
		StopObject->ChangeMaterialToDefault();
		StopObject->SetbIsDetected(false);
	}
	
}

