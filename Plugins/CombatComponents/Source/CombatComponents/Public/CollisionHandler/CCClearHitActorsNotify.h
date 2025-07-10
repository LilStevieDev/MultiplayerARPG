// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "CCClearHitActorsNotify.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "CC_ClearHitActors"))
class COMBATCOMPONENTS_API UCCClearHitActorsNotify : public UAnimNotify_PlayMontageNotify
{
	GENERATED_BODY()
	
public:

	/* constructor */
	UCCClearHitActorsNotify();

	/* overridden method */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const override;
};

