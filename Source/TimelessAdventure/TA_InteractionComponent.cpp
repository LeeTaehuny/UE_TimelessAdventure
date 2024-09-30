// Fill out your copyright notice in the Description page of Project Settings.


#include "TA_InteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Interface/InteractableInterface.h"
#include "KismetTraceUtils.h"
#include "Kismet/GameplayStatics.h"

UTA_InteractionComponent::UTA_InteractionComponent()
{
}

void UTA_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetWorld())
	{
		FTimerHandle TraceTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &UTA_InteractionComponent::PerformInteractionTrace, 0.2f, true);

	}
}

void UTA_InteractionComponent::InteractionPositive()
{
	
	//AActor* Owner = GetOwner();
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	//UE_LOG(LogTemp, Display, TEXT("InteractionPositive"));
	//if(OwnerCharacter)
	//	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetInstigatorController());
		//if(PlayerController)
	if(CachedInteractableActor)
	{
		IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(CachedInteractableActor);
		InteractableActor->Interact();
	}

			/*
			if(GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_GameTraceChannel1, _TraceParams))
			{
				AActor* HitActor = _HitOut.GetActor();
				if(HitActor != nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HitActor:%s"), *_HitOut.GetActor()->GetName()));
			
					DrawDebugLine(GetWorld(), _Start, _HitOut.ImpactPoint, FColor::Red, false, 5.f, 0, 2.f);
					DrawDebugSphere(GetWorld(), _HitOut.ImpactPoint, 10.f, 12, FColor::Yellow, false, 5.f);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HitLocation:%s"), *_HitOut.ImpactPoint.ToString()));

				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No, Hit"));
				DrawDebugLine(GetWorld(), _Start, _End, FColor::Blue, false, 5.f, 0, 2.f);
			}
			*/
		

	
}

void UTA_InteractionComponent::PerformInteractionTrace()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("performinteractiontrace"));
	}
	FVector _Start = OwnerCharacter->GetActorLocation();
	FVector _End = _Start + OwnerCharacter->GetActorForwardVector() * 2000.0f;
	FHitResult _HitOut;

	FCollisionQueryParams _TraceParams;
	_TraceParams.AddIgnoredActor(OwnerCharacter);


	bool bHit = GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_GameTraceChannel1, _TraceParams);
	if(bHit)
	{
		AActor* HitActor = _HitOut.GetActor();
		if(HitActor)
		{

			IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(HitActor);
			if(InteractableActor)
			{
				if(CachedInteractableActor != HitActor)
				{
					if(CachedInteractableActor)
					{
						IInteractableInterface* CachedInteractable = Cast<IInteractableInterface>(CachedInteractableActor);
						if(CachedInteractable)
						{
							CachedInteractable->HideInteractionWidget();
						}
					}
					CachedInteractableActor = HitActor;
					InteractableActor->DisplayInteractionWidget();
				}
			}
		}
	}
	else
	{
		if(CachedInteractableActor)
		{
			IInteractableInterface* CachedInteractable = Cast<IInteractableInterface>(CachedInteractableActor);
			if(CachedInteractable)
			{
				CachedInteractable->HideInteractionWidget();
			}
			CachedInteractableActor = nullptr;
		}
	}
}

void UTA_InteractionComponent::UpdateMoney(int64 inputval)
{
	int64 _result;
	_result = CurrentMoney + inputval;
	if(_result < 0)
	{
		
	}
	else
	{
		CurrentMoney = _result;
	}
}


