// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#include "CollisionHandler/CCActivateCollisionNotifyWindow.h"
#include "Components/SkeletalMeshComponent.h"

UCCActivateCollisionNotifyWindow::UCCActivateCollisionNotifyWindow()
	: CollisionPart(ECCCollisionPart::PrimaryItem)
{
	NotifyName = TEXT("ActivateCollision");
}

// activate collision on notify begin
void UCCActivateCollisionNotifyWindow::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp)
	{
		if ( AActor* Owner = MeshComp->GetOwner() )
		{
			if( UCCCollisionHandlerComponent* CollisionHandlerComponent = Cast<UCCCollisionHandlerComponent>( Owner->GetComponentByClass( UCCCollisionHandlerComponent::StaticClass() ) ) )
			{
				CollisionHandlerComponent->ActivateCollision( CollisionPart );
			}
		}
	}
}

// deactivate collision on notify end
void UCCActivateCollisionNotifyWindow::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp)
	{
		if ( AActor* Owner = MeshComp->GetOwner() )
		{
			if( UCCCollisionHandlerComponent* CollisionHandlerComponent = Cast<UCCCollisionHandlerComponent>( Owner->GetComponentByClass( UCCCollisionHandlerComponent::StaticClass() ) ) )
			{
				CollisionHandlerComponent->DeactivateCollision();
			}
		}
	}
}

FString UCCActivateCollisionNotifyWindow::GetNotifyName_Implementation() const
{
	return "ActColl";
}

