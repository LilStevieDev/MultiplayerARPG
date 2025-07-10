// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#include "RotatingOwner/CCRotateOwnerNotify.h"
#include "RotatingOwner/CCRotatingOwnerComponent.h"
#include "Components/SkeletalMeshComponent.h"

UCCRotateOwnerNotify::UCCRotateOwnerNotify()
	: DegreesPerSecond(540.f), MaxPossibleRotation(180.f)
{
	NotifyName = TEXT("RotateOwner");
}

void UCCRotateOwnerNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		if ( AActor* Owner = MeshComp->GetOwner() )
		{
			if( UCCRotatingOwnerComponent* RotatingComponent = Cast<UCCRotatingOwnerComponent>(Owner->GetComponentByClass(UCCRotatingOwnerComponent::StaticClass())) )
			{
				RotatingComponent->StartRotatingWithLimit(MaxPossibleRotation, DegreesPerSecond);
			}
		}
	}
}

FString UCCRotateOwnerNotify::GetNotifyName_Implementation() const
{
	return "RotateOwner";
}
