// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Monster/AnimNotify_Dying.h"
#include "Game/TA_MainGameMode.h"

#include "Kismet/GameplayStatics.h"

void UAnimNotify_Dying::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ATA_MainGameMode* GM = Cast<ATA_MainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->DeathBoss();
	}
}
