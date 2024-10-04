// Fill out your copyright notice in the Description page of Project Settings.
#include "Gimmick/Sangeon/TA_GrapRotateComponent.h"

#include <tiffio.h>

#include "TA_GrapCrosshair.h"
#include "TA_Movable.h"
#include "VectorUtil.h"
#include "Blueprint/UserWidget.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/TA_PlayerCharacter.h"

// Sets default values

UTA_GrapRotateComponent::UTA_GrapRotateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	LerpTimelineLength = 1.0f;
	LerpCurve = NewObject<UCurveFloat>();
	if(LerpCurve)
	{
		LerpCurve->FloatCurve.AddKey(0.0f, 0.0f);
		LerpCurve->FloatCurve.AddKey(0.1f, 1.0f);

		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("LerpProgress"));
		LerpTimeline.AddInterpFloat(LerpCurve,ProgressFunction);
	}
}

void UTA_GrapRotateComponent::BeginPlay()
{
	Super::BeginPlay();
	if(LerpCurve)
	{
		//FOnTimelineFloat CurveCallback;
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("LerpProgress"));
		LerpTimeline.AddInterpFloat(LerpCurve, ProgressFunction);

		FOnTimelineEvent TimelineFInished;
		TimelineFInished.BindUFunction(this, FName("LerpFinish"));
		LerpTimeline.SetTimelineFinishedFunc(TimelineFInished);
	}

}
void UTA_GrapRotateComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);
	LerpTimeline.TickTimeline(DeltaTime);
}


void UTA_GrapRotateComponent::updatewidget()
{
	if(!GetWorld())
	{
		return;
	}
	if(WidgetClass)
	{

		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

		if(WidgetInstance)
		{

			CrosshairWidget = WidgetInstance;
			CrosshairWidget->AddToViewport();

			GetWorld()->GetTimerManager().SetTimer(
				CrosshairTimerHandle,
				this,
				&UTA_GrapRotateComponent::TraceValidTarget,
				0.1f,
				true
			);
			
		}
	}
	else
	{
	}
}

void UTA_GrapRotateComponent::TraceValidTarget()
{
	FHitResult _Hit;
	bool bLineTraceHit;
	
	LineTraceFromCamera(_Hit, bLineTraceHit);
	UTA_GrapCrosshair* GrapCrosshair = Cast<UTA_GrapCrosshair>(CrosshairWidget);
	if(!GrapCrosshair)
	{
		return;
	}
	if(bLineTraceHit)
	{
		AActor* HitActor = _Hit.GetActor();
		//hit
		if(HitActor)
		{
			ATA_Movable* MovableActor = Cast<ATA_Movable>(HitActor);
			//cast success
			if(MovableActor)
			{
				if(!MovableActor->IsLocked)
				{
					if(!GrapCrosshair->IsHovered())
					{
						GrapCrosshair->Hover();
					}
				}
			}
			
			//cast success
			else
			{
				if(GrapCrosshair->IsHovered())
				{
					GrapCrosshair->UnHover();
				}
			}
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Nocollision"));
		if(GrapCrosshair->IsHovered())
		{
			GrapCrosshair->UnHover();
		}
	}
	
}

void UTA_GrapRotateComponent::LineTraceFromCamera(FHitResult& OutHit, bool& bHit)
{
	ATA_PlayerCharacter* PlayerCharacter = Cast<ATA_PlayerCharacter>(GetOwner());
	if(!PlayerCharacter)
	{
		return;
	}
	if(!GetWorld())
	{
		return;
	}
	StartLocation = PlayerCharacter->GetCameraComponent()->GetComponentLocation();
	FVector ForwardVector = PlayerCharacter->GetCameraComponent()->GetForwardVector();

	float TraceDistance = 1000.0f;
	EndLocation = StartLocation + (ForwardVector * TraceDistance);

	//FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerPlayer);

	bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		Params
		
	);
	//UE_LOG(LogTemp, Warning, TEXT("%s"),bHit ? TEXT("true") : TEXT("false"));

}

void UTA_GrapRotateComponent::PickupObject()
{
	FHitResult _Hit;
	bool bLineTraceHit;
	LineTraceFromCamera(_Hit, bLineTraceHit);
	ATA_PlayerCharacter* Player = Cast<ATA_PlayerCharacter>(GetOwner());
	if(!Player)
	{
		return;
	}
	if(bLineTraceHit)
	{

		AActor* HitActor = _Hit.GetActor();

		ATA_Movable* MovableActor = Cast<ATA_Movable>(HitActor);
		ATA_PlayerCharacter* PlayerCharacter = Cast<ATA_PlayerCharacter>(Player);
		

		if(MovableActor)
		{
			//UE_LOG(LogTemp, Warning, TEXT("pickup"));

			bool bPickupSuccess = MovableActor->SelfPickup(PlayerCharacter->GetCameraComponent());
			if(bPickupSuccess)
			{
				UStaticMeshComponent* MeshComponent = MovableActor->FindComponentByClass<UStaticMeshComponent>();
				
				if(MeshComponent == nullptr)
				{
					//UE_LOG(LogTemp, Warning, TEXT("meshcomp is null"));
					return;
				}
				HeldObject = MovableActor;
				FVector CurrentLocation = HeldObject->GetActorLocation();
				
				StartLerping(CurrentLocation);
				GetWorld()->GetTimerManager().ClearTimer(CrosshairTimerHandle);
				UTA_GrapCrosshair* GrapCrosshair = Cast<UTA_GrapCrosshair>(CrosshairWidget);
				GrapCrosshair->Hover();
				FVector StartingBounds = SetObjectBounds();
				//StartingDistance = FVector::Dist(FVector(0,0,0), HeldObject->GetRelativeLocation());
				//StartingScale = HeldObject->GetComponentScale();
			}
		}
		else
		{
			return;
		}
	}


	
}

FVector UTA_GrapRotateComponent::SetObjectBounds()
{
	FVector BoxExtent = FVector::ZeroVector;
	if(HeldObject)
	{
		FVector Origin;
//		float SphereRadius;
		HeldObject->GetActorBounds(false,Origin, BoxExtent);
		
		//BoxExtent = HeldObject->Bounds.BoxExtent;
	}
	return BoxExtent;
}

void UTA_GrapRotateComponent::StartLerping(FVector NewStartLocation)
{
	LerpStartLocation = NewStartLocation;
	if(LerpCurve)
	{
		LerpTimeline.PlayFromStart();
		
	}
	
	
}
void UTA_GrapRotateComponent::LerpProgress(float Value)
{
	if(HeldObject)
	{
		float Distance = FVector::Dist(StartLocation, FVector(0, 0, 0));
		FVector A = LerpStartLocation;
		FVector B = Distance* FVector(1,0,0);
		FVector NewLocation = FMath::Lerp(A, B, Value);
		HeldObject->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}
void UTA_GrapRotateComponent::DropObject()
{
	
	if(HeldObject)
	{
		//ATA_PlayerCharacter* Player = Cast<ATA_PlayerCharacter>(GetOwner());
		//AActor* Owner = GetOwner();
		//if(Owner)
		//{
		ATA_Movable* MovableActor = Cast<ATA_Movable>(HeldObject);
		if(MovableActor)
		{
			MovableActor->SelfDrop();
			UTA_GrapCrosshair* GrapCrosshair = Cast<UTA_GrapCrosshair>(CrosshairWidget);
			HeldObject = NULL;
			GrapCrosshair->UnHover();
			GetWorld()->GetTimerManager().SetTimer(
				CrosshairTimerHandle,
				this,
				&UTA_GrapRotateComponent::TraceValidTarget,
				0.1f,
				true
			);
		}
		
	}
	else
	{
	}
}

void UTA_GrapRotateComponent::RotateObject(float RotationValue)
{
	//ATA_PlayerCharacter* Player = Cast<ATA_PlayerCharacter>(GetOwner());
	//if(Player)
	//{
	/*
	if(HeldObject)
	{

		FRotator CurrentRotation = HeldObject->GetActorRotation();
		//UE_LOG(LogTemp, Warning, TEXT("Current Rotation: %s"), *CurrentRotation.ToString());
		
		FRotator CurrentRotationX = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, FMath::Fmod(CurrentRotation.Roll + RotationValue, 360.0f));
		FRotator CurrentRotationY = FRotator(FMath::Fmod(CurrentRotation.Pitch + RotationValue, 360.0f), CurrentRotation.Yaw, CurrentRotation.Roll);
		FRotator CurrentRotationZ = FRotator(CurrentRotation.Pitch, FMath::Fmod(CurrentRotation.Yaw + RotationValue, 360.0f), CurrentRotation.Roll + RotationValue);

		FRotator SelectedRotator1 = bShiftHeld ? CurrentRotationY : CurrentRotationZ;
		FRotator SelectedRotator2 = bctrlheld ? CurrentRotationX : SelectedRotator1;
		NewRotator = SelectedRotator2;
		//UE_LOG(LogTemp, Warning, TEXT("Current Rotation: %s"), *SelectedRotator2.ToString());

		HeldObject->SetActorRotation(NewRotator);
	}
	*/
	if (HeldObject)
	{
		// 현재 회전을 쿼터니언으로 변환
		FQuat CurrentRotation = HeldObject->GetActorQuat();
        
		// 회전할 축을 설정 (X, Y, Z에 따라 다르게)
		FVector RotationAxis;
        
		if (bShiftHeld) // X축 회전
		{
			RotationAxis = FVector(1.0f, 0.0f, 0.0f);
		}
		else if (bctrlheld) // Z축 회전
		{
			RotationAxis = FVector(0.0f, 0.0f, 1.0f);
		}
		else // Y축 회전
		{
			RotationAxis = FVector(0.0f, 1.0f, 0.0f);
		}

		// 쿼터니언으로 회전 값 적용
		FQuat DeltaRotation = FQuat(RotationAxis, FMath::DegreesToRadians(RotationValue));
		FQuat NewRotation = CurrentRotation * DeltaRotation;

		// 회전 값 적용
		HeldObject->SetActorRotation(NewRotation);
	}
}

