// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TA_Bow.h"
#include "Item/TA_Arrow.h"

#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

ATA_Bow::ATA_Bow()
{
	WeaponType = EWeaponType::WT_Bow;

	WeaponSocketName = TEXT("BowSocket");
}

void ATA_Bow::BeginPlay()
{
	Super::BeginPlay();

	// 화살 줄의 로컬 위치를 구해줍니다.
	// * GetTransform().InverseTransformPosition(소켓 위치) : 월드 상의 소켓 위치를 월드변환행렬의 역함수를 곱해 로컬 위치를 구합니다.
	BaseLocation = GetTransform().InverseTransformPosition(WeaponMesh->GetSocketLocation(StringSocketName));
}

void ATA_Bow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsHold)
	{
		// 화살 위치를 전달받아 StringLocation에 저장합니다.
		StringLocation = GetArrowSocketLocation(TempMesh);
		// 화살 줄 Bone의 위치를 당겨진 상태로 설정합니다.
		WeaponMesh->SetBoneLocationByName(StringSocketName, StringLocation, EBoneSpaces::WorldSpace);
	}
	else
	{
		// 화살 줄 Bone의 위치를 기본 위치로 설정합니다.
		WeaponMesh->SetBoneLocationByName(StringSocketName, BaseLocation, EBoneSpaces::ComponentSpace);
	}
}

void ATA_Bow::SpawnArrow(USkeletalMeshComponent* Mesh)
{
	if (ArrowClass)
	{
		Arrow = GetWorld()->SpawnActor<ATA_Arrow>(ArrowClass);
		if (Arrow)
		{
			Arrow->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ArrowSocketName);
		}
	}
}

void ATA_Bow::ShootArrow()
{
	if (Arrow)
	{
		// 화면 중앙으로 라인 트레이스를 진행하여 총알이 충돌할 위치를 구해줄 예정입니다.
		// * 뷰포트의 크기를 가져옵니다.
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		// * 뷰포트의 중앙 좌표를 구해줍니다. (Screen 좌표)
		FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
		// * Screen 좌표로부터 World 기준 좌표와 방향을 구해주도록 합니다.
		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld
		(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation,
			CrosshairWorldPosition,
			CrosshairWorldDirection
		);

		// 스크린의 중앙 좌표를 구한 경우
		if (bScreenToWorld)
		{
			FHitResult HitResult;
			FVector Start = CrosshairWorldPosition;
			FVector End = Start + CrosshairWorldDirection * 2000.0f;

			// 라인 트레이스 진행
			bool bIsSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);

			if (bIsSuccess)
			{
				//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 12, FColor::Red, true);

				Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				Arrow->FireArrow(HitResult.ImpactPoint);
				Arrow = nullptr;
				return;
			}
		}

		Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Arrow->FireArrow(CrosshairWorldPosition + CrosshairWorldDirection * 2000.0f);
		Arrow = nullptr;
	}
}

void ATA_Bow::RemoveArrow()
{
	if (Arrow)
	{
		Arrow->Destroy();
	}
}

void ATA_Bow::RemoveWeapon()
{
	if (Arrow)
	{
		Arrow->Destroy();
	}

	if (Quiver)
	{
		Quiver->Destroy();
	}

	Super::RemoveWeapon();
}

FVector ATA_Bow::GetArrowSocketLocation(USkeletalMeshComponent* Mesh)
{
	// 화살 소켓 위치를 반환합니다.
	return Mesh->GetSocketLocation(ArrowSocketName);
}

void ATA_Bow::EquipWeapon(USkeletalMeshComponent* Mesh)
{
	Super::EquipWeapon(Mesh);

	// 화살통 스폰
	Quiver = GetWorld()->SpawnActor<AActor>(QuiverClass, GetActorTransform());
	Quiver->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, QuiverSocketName);
}
