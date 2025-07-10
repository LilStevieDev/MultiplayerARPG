// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#include "RotatingOwner/CCRotateOwnerNotifyWindow.h"
#include "RotatingOwner/CCRotatingOwnerComponent.h"
#include "Components/SkeletalMeshComponent.h"

UCCRotateOwnerNotifyWindow::UCCRotateOwnerNotifyWindow()
	: DegreesPerSecond( 540.f )
{
	NotifyName = TEXT( "RotateOwner" );
}

void UCCRotateOwnerNotifyWindow::NotifyBegin( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration )
{
	if( MeshComp )
	{
		if( AActor* Owner = MeshComp->GetOwner() )
		{
			if( UCCRotatingOwnerComponent* RotatingComponent = Cast<UCCRotatingOwnerComponent>( Owner->GetComponentByClass( UCCRotatingOwnerComponent::StaticClass() ) ) )
			{
				RotatingComponent->StartRotating( 10.f, DegreesPerSecond );
			}
		}
	}
}

void UCCRotateOwnerNotifyWindow::NotifyEnd( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( MeshComp )
	{
		if( AActor* Owner = MeshComp->GetOwner() )
		{
			if( UCCRotatingOwnerComponent* RotatingComponent = Cast<UCCRotatingOwnerComponent>( Owner->GetComponentByClass( UCCRotatingOwnerComponent::StaticClass() ) ) )
			{
				RotatingComponent->StopRotating();
			}
		}
	}
}

FString UCCRotateOwnerNotifyWindow::GetNotifyName_Implementation() const
{
	return "RotateOwner";
}