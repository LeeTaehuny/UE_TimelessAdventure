// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Equip.h"

#include "TA_CombatComponent.h"
#include "GameFramework/Character.h"

void UAnimNotify_Equip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	// 캐릭터에서 CombatComponent 가져오기
	ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		UTA_CombatComponent* CombatComponent = Cast<UTA_CombatComponent>(OwnerCharacter->GetComponentByClass(UTA_CombatComponent::StaticClass()));
		if (CombatComponent)
		{
			// 무기를 장착하는 동작을 여기서 실행
			CombatComponent->OnNotifyReceived(nullptr);  // 여기서 애니메이션 노티파이와 함께 무기를 장착
			UE_LOG(LogTemp, Warning, TEXT("Weapon equipped via AnimNotify"));
		}
	}
}
