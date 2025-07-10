// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "CCRotateOwnerNotify.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "CC_RotOwner"))
class COMBATCOMPONENTS_API UCCRotateOwnerNotify : public UAnimNotify_PlayMontageNotify
{
	GENERATED_BODY()
	
public:

	/* constructor */
	UCCRotateOwnerNotify();

	/* overridden method */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const override;

	/* Limit of degrees to rotate per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotating")
	float DegreesPerSecond;

	/**
	 * Rotating time will be calculated based on that value: Time = MaxPossibleRotation/DegressPerSecond
	 * Example: MaxPossibleRotation=180, DegressPerSecond=360 -> rotation will be activated for 0.5 sec
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotating")
	float MaxPossibleRotation;
	
};
