// Fill out your copyright notice in the Description page of Project Settings.


#include "HR_StateChange.h"

#include "Component/TA_InputComponent.h"
#include "Components/Button.h"
#include "HR/HR_StopAbilityComponent_T.h"
#include "Player/TA_PlayerCharacter.h"
#include "Player/TA_PlayerController.h"


void UHR_StateChange::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Combat = Cast<UButton>(GetWidgetFromName("BTN_Combat"));
	BTN_StopObject = Cast<UButton>(GetWidgetFromName("BTN_StopObject"));
	BTN_GrapObject = Cast<UButton>(GetWidgetFromName("BTN_GrapObject"));

	BTN_Combat->OnClicked.AddDynamic(this,&UHR_StateChange::OnClickedCombat);
	BTN_StopObject->OnClicked.AddDynamic(this,&UHR_StateChange::OnClickedStopObject);
	BTN_GrapObject->OnClicked.AddDynamic(this,&UHR_StateChange::OnClickedGrapObject);

}

bool UHR_StateChange::Initialize()
{
	bool Result = Super::Initialize();
	
	// 바인딩

	if (!BTN_Combat || !BTN_StopObject || BTN_GrapObject) return false;

	BTN_Combat->OnClicked.AddDynamic(this,&UHR_StateChange::OnClickedCombat);
	BTN_StopObject->OnClicked.AddDynamic(this,&UHR_StateChange::OnClickedStopObject);
	BTN_GrapObject->OnClicked.AddDynamic(this,&UHR_StateChange::OnClickedGrapObject);

	return Result;

}

void UHR_StateChange::OnClickedCombat()
{
	// State 변경
	// 유아이 Invisible
	ATA_PlayerCharacter* player = Cast<ATA_PlayerCharacter>(OwnerActor);
	if (player)
	{
		if(IsValid(player->GetInputComponent()))
		{
			player->GetInputComponent()->ChangeStateToCombat();
		}
		// UI 닫기
		ATA_PlayerController* pc = Cast<ATA_PlayerController>(player->GetController());
		if(pc)
		{
			pc->SetVisibleStateChangeWidget(false);
			pc->SetShowMouseCursor(false);
		}
	}

	
}

void UHR_StateChange::OnClickedStopObject()
{
	// State 변경
	// StopAbility의 Stopobjectbegin호출
	ATA_PlayerCharacter* player = Cast<ATA_PlayerCharacter>(OwnerActor);
	if (player)
	{
		if(IsValid(player->GetInputComponent()) && IsValid(player->GetStopAbilityComponent()))
		{
			player->GetInputComponent()->ChangeStateToStopObject();
			player->GetStopAbilityComponent()->StopAbilityBegin();
		}
		// UI 닫기
		ATA_PlayerController* pc = Cast<ATA_PlayerController>(player->GetController());
		if(pc)
		{
			pc->SetVisibleStateChangeWidget(false);
		}
	}

}

void UHR_StateChange::OnClickedGrapObject()
{
	// State 변경
	ATA_PlayerCharacter* player = Cast<ATA_PlayerCharacter>(OwnerActor);
	if (player)
	{
		if(IsValid(player->GetInputComponent()))
		{
			player->GetInputComponent()->ChangeStateToGrabObject();
		}
		// UI 닫기
		ATA_PlayerController* pc = Cast<ATA_PlayerController>(player->GetController());
		if(pc)
		{
			pc->SetVisibleStateChangeWidget(false);
			pc->SetShowMouseCursor(false);
		}
	}

}

