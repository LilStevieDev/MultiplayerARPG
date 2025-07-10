// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/HitResult.h"
#include "Components/ActorComponent.h"
#include "CCCollisionHandlerComponent.generated.h"

/**
 * Stores informations about component which will cause collision such as:
 * Component - primitive component e.g Static Mesh / Skeletal Mesh etc.
 * Sockets - name of sockets attached to given Component, used to retrieve its location
 * HitActors (not exposed to Blueprints) - actors that were hit during single collision activation
 */
USTRUCT(BlueprintType)
struct FCCCollidingComponent
{
	GENERATED_BODY()

	/* Default constructor */
	FCCCollidingComponent() 
		:	Component( nullptr )
		{};

	/* Constructor taking params */
	FCCCollidingComponent(UPrimitiveComponent* component, TArray<FName> sockets)
		:	Component(component),
			Sockets(sockets)
		{
			// if doesn't have any sockets
			// add default one which will represent component world location
			if( Sockets.Num() == 0 )
			{
				Sockets.Add( NAME_None );
			}
		}

	/* Component which will be used to find socket locations from e.g Sword Static Mesh */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CollidingComponent" )
	UPrimitiveComponent* Component;

	/* Socket names that should exist in given Component in order to retrieve their locations */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CollidingComponent" )
	TArray<FName> Sockets;

	/* Hidden property that stores hit actors by this Component */
	UPROPERTY( BlueprintReadOnly, Category = "CollidingComponent" )
	TArray<AActor*> HitActors;

	/** Returns location on component by given socket name */
	FVector GetSocketLocation( const FName& SocketName ) const;

	/* Override == operator to compare these structs on its Component pointer */
	FORCEINLINE bool operator == (const FCCCollidingComponent& other) const
	{
		return this->Component == other.Component;
	}
};

/**
 * Enum which helps to determine on which part of body or weapon collision should be activated.
 * Example: When collision is activated, switch colliding component and its sockets based on ECollisionPart value
 * if it is PrimaryItem, update CollidingComponent to sword, if it is LeftArm update it to left arm of the character etc.
 */
UENUM(BlueprintType)
enum class ECCCollisionPart : uint8
{					// Examples
	NONE,
	PrimaryItem,	// Sword in Right Hand
	SecondaryItem,	// Shield in Left Hand
	BothHandItems,	// Both dual weapons
	LeftArm,		// Left hand of the character
	RightArm,		// Rigth hand of the character
	LeftLeg,		// Left foot of the character
	RightLeg,		// Right foot of the character
	Custom1,		
	Custom2,
	Custom3
};

/* Delegate called when there was a collision */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHit, const FHitResult&, HitResult, UPrimitiveComponent*, CollidingComponent);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHitNative, FHitResult, UPrimitiveComponent*);

/* Delegate called when collision was activated */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollisionActivated, ECCCollisionPart, CollisionPart);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCollisionActivatedNative, ECCCollisionPart);

/* Delegate called when collision was deactivated */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionDeactivated);
DECLARE_MULTICAST_DELEGATE(FOnCollisionDeactivatedNative);

/**
 * Component which allows to setup in a simple way accurate collision checks.
 * It works on the principle of checking whether there is any actor between the socket position in this and the previous frame.
 * May be used on Sword Static Mesh, Hand of Character Mesh, Arrow Projectile etc.
 */
UCLASS( ClassGroup=(CombatComponents), meta=(BlueprintSpawnableComponent) )
class COMBATCOMPONENTS_API UCCCollisionHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* Default constructor */
	UCCCollisionHandlerComponent();

protected:
	/* Called when the game starts */
	virtual void BeginPlay() override;




	/************************************************************************/
	/*								SETTINGS								*/
	/************************************************************************/
public:
	/* Whether to use trace complex option during trace check or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandlerComponent")
	uint32 bTraceComplex : 1;

	/* Radius of sphere trace checking collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandlerComponent")
	float TraceRadius;

	/* How often there is trace check while collision is activated */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandlerComponent")
	float TraceCheckInterval;

	/* Classes that will be ignored while checking collision, may be friendly AI etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandlerComponent")
	TArray<TSubclassOf<AActor>> IgnoredClasses;

	/** 
	 * Profile names that components will be ignored with.
	 * May be useful to ignore collision capsule which profile name is 'Pawn' 
	 * and collide only with its Character Mesh which profile name is 'CharacterMesh'
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandlerComponent")
	TArray<FName> IgnoredCollisionProfileNames;

	/* Types of objects to collide with - Pawn, WordStatic etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandlerComponent")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToCollideWith;

	/**
	 * Actors which should always be ignored performing trace checks
	 * Means that they will never be hit
	 */
	UPROPERTY( BlueprintReadWrite, Category = "CollisionHandlerComponent")
	TArray<AActor*> IgnoredActors;

	/* Checks whether given class should be ignored or not */
	bool IsIgnoredClass( TSubclassOf<AActor> actorClass );

	/* Checks whether given profile name is ignored or not*/
	bool IsIgnoredProfileName( FName profileName );
	/************************************************************************/





	/************************************************************************/
	/*								ACTIVATION								*/
	/************************************************************************/
public:
	/* Stores value of currently activated CollisionPart */
	UPROPERTY( Replicated, BlueprintReadOnly, Category = "CollisionHandlerComponent" )
	ECCCollisionPart ActivatedCollisionPart;

	/* Array storing colliding components */
	UPROPERTY( BlueprintReadOnly, Category = "CollisionHandlerComponent" )
	TArray<FCCCollidingComponent> ActiveCollidingComponents;


	/* Activates collision */
	UFUNCTION(BlueprintCallable, Category = "CollisionHandlerComponent")
	void ActivateCollision(ECCCollisionPart collisionPart);

	/* Deactivates collision */
	UFUNCTION(BlueprintCallable, Category = "CollisionHandlerComponent")
	void DeactivateCollision();

	/* Returns true if collision is activated, false otherwise */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CollisionHandlerComponent")
	bool IsCollisionActivated() const { return bIsCollisionActivated; }

	/* Set new colliding component and sockets array that should be associated with it */
	UFUNCTION(BlueprintCallable, Category = "CollisionHandlerComponent")
	void UpdateCollidingComponent(UPrimitiveComponent* component, const TArray<FName>& sockets);

	/* Set new colliding components and sockets array that should be associated with it */
	UFUNCTION(BlueprintCallable, Category = "CollisionHandlerComponent")
	void UpdateCollidingComponents(const TArray<FCCCollidingComponent>& collidingComponents);

	UFUNCTION( BlueprintCallable, Category = "CollisionHandlerComponent" )
	void SetActiveCollisionPart( ECCCollisionPart CollisionPart );

	/* Returns currently activated collision part type */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CollisionHandlerComponent")
	ECCCollisionPart GetActivatedCollisionPart() const { return ActivatedCollisionPart; }

	/* Clears HitActors array for each colliding component */
	UFUNCTION(BlueprintCallable, Category = "CollisionHandlerComponent")
	void ClearHitActors();


protected:
	/* Determines whether collision is activated */
	UPROPERTY(ReplicatedUsing = "OnRep_IsCollisionActivated")
	uint32 bIsCollisionActivated : 1;

	/* Handle for trace check loop timer */
	UPROPERTY()
	FTimerHandle TimerHandle_TraceCheck;

	/**
	 * Map storing location of sockets in last frame.
	 * For example:	SteelSwordSocket01, FVector(12,32,0)
	 *				SteelSwordSocket02, FVector(12,32,5) etc.
	 */
	UPROPERTY()
	TMap<FName, FVector> LastFrameSocketLocations;

	UFUNCTION()
	void OnRep_IsCollisionActivated();

	/* Function called on timer to perform trace check */
	UFUNCTION()
	void TraceCheckLoop();

	/**
	 * Determines whether trace check can be performed.
	 * Used to make sure it won'tt happen on first timer tick to firstly store socket locations.
	 */
	uint32 bCanPerformTrace : 1;

	/* Updates location values in sockets */
	void UpdateSocketLocations();

	/**
	 * Does a sphere trace between socket locations in last and current frame,
	 * and check whether there is any colliding object between these locations
	 */
	void PerformTraceCheck();

	/**
	 * Generates unique socket name based on given component and socket name e.g 'SteelSwordCollidingSocket01'.
	 * Used to differentiate components if they are using same socket names.
	 */
	FName GenerateUniqueSocketName( UPrimitiveComponent* collidingComponent, FName socket );
	/************************************************************************/

	

	


	/************************************************************************/
	/*								EVENTS								*/
	/************************************************************************/
public:
	/* Delegate called when there was a collision */
	UPROPERTY(BlueprintAssignable, Category = "CollisionHandlerComponent")
	FOnHit OnHit;

	/* Native version above, called before BP delegate */
	FOnHitNative OnHitNative;

	/* Delegate called when collision was activated */
	UPROPERTY(BlueprintAssignable, Category = "CollisionHandlerComponent")
	FOnCollisionActivated OnCollisionActivated;

	/* Native version above, called before BP delegate */
	FOnCollisionActivatedNative OnCollisionActivatedtNative;

	/* Delegate called when collision was deactivated */
	UPROPERTY(BlueprintAssignable, Category = "CollisionHandlerComponent")
	FOnCollisionDeactivated OnCollisionDeactivated;

	/* Native version above, called before BP delegate */
	FOnCollisionDeactivatedNative OnCollisionDeactivatedNative;

protected:
	/* Calls the callback */
	void NotifyOnHit( const FHitResult& hitResult, UPrimitiveComponent* collidingComponent );

	/* Calls the callback */
	void NotifyOnCollisionActivated( ECCCollisionPart collisionPart );

	/* Calls the callback */
	void NotifyOnCollisionDeactivated();
	/************************************************************************/





	/************************************************************************/
	/*								DEBUG								*/
	/************************************************************************/
public:
	/* Debug property exists only with editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandlerComponent")
	uint32 bDebug : 1;

private:
	UFUNCTION()
	void DrawHitSphere(FVector location);

	UFUNCTION()
	void DrawDebugTrace(FVector start, FVector end);
	/************************************************************************/

};
