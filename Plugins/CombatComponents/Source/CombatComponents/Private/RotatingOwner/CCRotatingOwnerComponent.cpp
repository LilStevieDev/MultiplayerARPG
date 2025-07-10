// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#include "RotatingOwner/CCRotatingOwnerComponent.h"
#include "RotatingOwner/CCRotatingOwnerInterface.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCCRotatingOwnerComponent::UCCRotatingOwnerComponent()
	: bIsRotating(false), DegreesPerSecond(540.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


void UCCRotatingOwnerComponent::NotifyOnRotatingStart()
{
	OnRotatingStartNative.Broadcast();
	OnRotatingStart.Broadcast();
}

void UCCRotatingOwnerComponent::NotifyOnRotatingEnd()
{
	OnRotatingEndNative.Broadcast();
	OnRotatingEnd.Broadcast();
}


void UCCRotatingOwnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get owner
	AActor* owner = GetOwner();

	if (bIsRotating)
	{
		// check if owner is valid and implements interface UCCRotatingOwnerInterface
		if (owner &&
			owner->GetClass()->ImplementsInterface(UCCRotatingOwnerInterface::StaticClass()))
		{
			// get desired rotation from owner through interface function GetDesiredRotation
			FRotator targetRotation = ICCRotatingOwnerInterface::Execute_GetDesiredRotation(owner);
			
			// get current owner rotation
			FRotator currentRotation = owner->GetActorRotation();
			
			// calculate new rotation by interpolating current and desired rotations
			FRotator newRotation = UKismetMathLibrary::RInterpTo_Constant(currentRotation, targetRotation, DeltaTime, DegreesPerSecond);

			// update elapsed time
			TimeElapsed += DeltaTime;
			if (TimeElapsed <= RotatingTime)
			{
				// update owner rotation to server and all clients
				SetOwnerRotation(newRotation);
			}
			else
			{
				// stop rotating if time has ended
				StopRotating();
			}
		}
		else
		{
			// stop rotation if owner is not valid or doesnt implement interface
			StopRotating();
		}
	}
}

void UCCRotatingOwnerComponent::SetOwnerRotation(const FRotator& newRotation)
{
	GetOwner()->SetActorRotation(newRotation);
}


void UCCRotatingOwnerComponent::StartRotating(float time, float degressPerSecond)
{
	// update variables
	RotatingTime = time;
	DegreesPerSecond = degressPerSecond;
	TimeElapsed = 0.f;
	bIsRotating = true;
	SetComponentTickEnabled( true );

	// call notify
	NotifyOnRotatingStart();
}

void UCCRotatingOwnerComponent::StartRotatingWithLimit(float maxPossibleRotation, float degressPerSecond)
{

	// calculate time and call StartRotating
	float time = maxPossibleRotation / degressPerSecond;
	StartRotating(time, degressPerSecond);
}

void UCCRotatingOwnerComponent::StopRotating()
{
	bIsRotating = false;
	SetComponentTickEnabled( false );

	// call notify
	NotifyOnRotatingEnd();
}
