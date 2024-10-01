// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StopAbilityComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UHR_StopAbilityComponent::UHR_StopAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game startss
void UHR_StopAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// player controller에서 Click 이벤트 사용 설정
	// 근데 default가 true인 것 같음..
	// ! pc는 생성자에서 받아오면 안됨
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	DeactivateMouseEvent();
	
}


// Called every frame
void UHR_StopAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHR_StopAbilityComponent::StopAbilityBegin()
{
	// Stop Object mode 시작
	// 1) 마우스 커서 보이기
	// 2) 마우스 이벤트 Active
	// 3) StopableObject를 관리하는 Manager를 통해서 Stopable Object들의 Material을 수정
	//		> Stopable Object를 TArray를 통해서 모든 Object material 변경
	//		> Component에 Collider를 붙여서 그 범위 안에 있는 Stopable Object들의 Material 변경
	PlayerController->bShowMouseCursor = true;
	ActivateMouseEvent();
	
	
}

void UHR_StopAbilityComponent::StopAbilityEnd()
{
	// Stop Object mode 종료
	// 1) 마우스 커서 끔
	// 2) 마우스 이벤트 deactive
	PlayerController->bShowMouseCursor = false;
	DeactivateMouseEvent();
	
}

void UHR_StopAbilityComponent::ActivateMouseEvent()
{
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
}

void UHR_StopAbilityComponent::DeactivateMouseEvent()
{
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;
}

