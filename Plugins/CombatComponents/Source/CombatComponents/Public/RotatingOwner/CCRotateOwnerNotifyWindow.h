// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "CCRotateOwnerNotifyWindow.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "CC_RotOwner"))
class COMBATCOMPONENTS_API UCCRotateOwnerNotifyWindow : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()
	
public:
	/* constructor */
	UCCRotateOwnerNotifyWindow();

	/* overriden functions */
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	FString GetNotifyName_Implementation() const override;

	/* Limit of degrees to rotate per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotating")
	float DegreesPerSecond;
	
};

