// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_WeaponComponent_bow.h"

UTA_WeaponComponent_bow::UTA_WeaponComponent_bow()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTA_WeaponComponent_bow::Attack()
{
	// 활 공격 로직 구현
	if (!bIsAttacking)
	{
		PlayAttackAnimation();  // 공통 애니메이션 재생 함수 호출
		bIsAttacking = true;
	}
}