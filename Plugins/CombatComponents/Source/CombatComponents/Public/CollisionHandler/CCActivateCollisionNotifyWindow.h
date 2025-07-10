// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "CCCollisionHandlerComponent.h"
#include "CCActivateCollisionNotifyWindow.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "CC_ActivateCollision"))
class COMBATCOMPONENTS_API UCCActivateCollisionNotifyWindow : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()
	
public:
	/* constructor */
	UCCActivateCollisionNotifyWindow();

	/* overriden functions */
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	FString GetNotifyName_Implementation() const override;

	/* Which CollisionPart should be passed when activating collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ECCCollisionPart CollisionPart;
	
};
