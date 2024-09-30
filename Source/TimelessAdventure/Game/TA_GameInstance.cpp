// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TA_GameInstance.h"
#include "Engine/DataTable.h"

UTA_GameInstance::UTA_GameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/LeeTaes/Data/DT_ItemDataTable.DT_ItemDataTable'"));
	if (DataTableRef.Object)
	{
		ItemDataTable = DataTableRef.Object;
	}
}

FItemData UTA_GameInstance::GetItemData(FName ItemName)
{
	return *ItemDataTable->FindRow<FItemData>(ItemName, TEXT(""));
}