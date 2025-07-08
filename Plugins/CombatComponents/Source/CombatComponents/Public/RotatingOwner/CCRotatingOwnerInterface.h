// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCRotatingOwnerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCCRotatingOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBATCOMPONENTS_API ICCRotatingOwnerInterface
{
	GENERATED_BODY()

public:
	/* Returns rotation that owner of the RotatingComponant should be rotated at while component is activated */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "RotatingOwnerComponent")
		FRotator GetDesiredRotation();
};
