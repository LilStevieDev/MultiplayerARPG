// Copyright 2021, Revilian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionQueryParams.h"
#include "TraceUtils.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULETRACEROTATION_API UTraceUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
		 * Sweeps a capsule along the given line and returns the first blocking hit encountered.
		 * This trace finds the objects that RESPOND to the given TraceChannel
		 *
		 * @param WorldContext	World context
		 * @param Start			Start of line segment.
		 * @param End			End of line segment.
		 * @param Radius		Radius of the capsule to sweep
		 * @param HalfHeight	Distance from center of capsule to tip of hemisphere endcap.
		 * @param Orientation	Capsule oriantation in world space
		 * @param TraceChannel
		 * @param bTraceComplex	True to test against complex collision, false to test against simplified collision.
		 * @param OutHit		Properties of the trace hit.
		 * @return				True if there was a hit, false otherwise.
		 */
	UFUNCTION(BlueprintCallable, Category = "Collision", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Capsule Trace By Channel With Rotation", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep"))
	static bool CapsuleTraceSingle(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);

	/**
		 * Sweeps a capsule along the given line and returns all hits encountered up to and including the first blocking hit.
		 * This trace finds the objects that RESPOND to the given TraceChannel
		 *
		 * @param WorldContext	World context
		 * @param Start			Start of line segment.
		 * @param End			End of line segment.
		 * @param Radius		Radius of the capsule to sweep
		 * @param HalfHeight	Distance from center of capsule to tip of hemisphere endcap.
		 * @param Orientation	Capsule oriantation in world space
		 * @param TraceChannel
		 * @param bTraceComplex	True to test against complex collision, false to test against simplified collision.
		 * @param OutHits		A list of hits, sorted along the trace from start to finish.  The blocking hit will be the last hit, if there was one.
		 * @return				True if there was a blocking hit, false otherwise.
		 */
	UFUNCTION(BlueprintCallable, Category = "Collision", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Multi Capsule Trace By Channel With Rotation", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep"))
	static bool CapsuleTraceMulti(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);

	// For Objects

	/**
	 * Sweeps a capsule along the given line and returns the first hit encountered.
	 * This only finds objects that are of a type specified by ObjectTypes.
	 *
	 * @param WorldContext	World context
	 * @param Start			Start of line segment.
	 * @param End			End of line segment.
	 * @param Radius		Radius of the capsule to sweep
	 * @param HalfHeight	Distance from center of capsule to tip of hemisphere endcap.
	 * @param Orientation	Capsule oriantation in world space
	 * @param ObjectTypes	Array of Object Types to trace
	 * @param bTraceComplex	True to test against complex collision, false to test against simplified collision.
	 * @param OutHit		Properties of the trace hit.
	 * @return				True if there was a hit, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Capsule Trace For Objects With Rotation", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep"))
	static bool CapsuleTraceSingleForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);

	/**
	 * Sweeps a capsule along the given line and returns all hits encountered.
	 * This only finds objects that are of a type specified by ObjectTypes.
	 *
	 * @param WorldContext	World context
	 * @param Start			Start of line segment.
	 * @param End			End of line segment.
	 * @param Radius		Radius of the capsule to sweep
	 * @param HalfHeight	Distance from center of capsule to tip of hemisphere endcap.
	 * @param Orientation	Capsule oriantation in world space
	 * @param ObjectTypes	Array of Object Types to trace
	 * @param bTraceComplex	True to test against complex collision, false to test against simplified collision.
	 * @param OutHits		A list of hits, sorted along the trace from start to finish.  The blocking hit will be the last hit, if there was one.
	 * @return				True if there was a hit, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Multi Capsule Trace For Objects With Rotation", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep"))
	static bool CapsuleTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);


	// BY PROFILE

	/**
	*  Sweep a capsule against the world and return the first blocking hit using a specific profile
	*
	* @param WorldContext	World context
	* @param Start			Start of line segment.
	* @param End			End of line segment.
	* @param Radius			Radius of the capsule to sweep
	* @param HalfHeight		Distance from center of capsule to tip of hemisphere endcap.
	* @param Orientation	Capsule oriantation in world space
	* @param ProfileName	The 'profile' used to determine which components to hit
	* @param bTraceComplex	True to test against complex collision, false to test against simplified collision.
	* @param OutHit			Properties of the trace hit.
	* @return				True if there was a hit, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "Collision", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Capsule Trace By Profile With Rotation", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep"))
	static bool CapsuleTraceSingleByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, UPARAM(Meta = (GetOptions = GetCollisionProfileNames)) FName ProfileName, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);

	/**
	*  Sweep a capsule against the world and return all initial overlaps using a specific profile, then overlapping hits and then first blocking hit
	*  Results are sorted, so a blocking hit (if found) will be the last element of the array
	*  Only the single closest blocking result will be generated, no tests will be done after that
	*
	* @param WorldContext	World context
	* @param Start			Start of line segment.
	* @param End			End of line segment.
	* @param Radius			Radius of the capsule to sweep
	* @param HalfHeight		Distance from center of capsule to tip of hemisphere endcap.
	* @param Orientation	Capsule oriantation in world space
	* @param ProfileName	The 'profile' used to determine which components to hit
	* @param bTraceComplex	True to test against complex collision, false to test against simplified collision.
	* @param OutHits		A list of hits, sorted along the trace from start to finish.  The blocking hit will be the last hit, if there was one.
	* @return				True if there was a blocking hit, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "Collision", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Multi Capsule Trace By Profile With Rotation", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep"))
	static bool CapsuleTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, UPARAM(Meta = (GetOptions = GetCollisionProfileNames)) FName ProfileName, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);



	static inline FCollisionQueryParams ConfigureCollisionParams(FName TraceTag, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, const UObject* WorldContextObject);
	static inline FCollisionObjectQueryParams ConfigureCollisionObjectParams(const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes);
	
};
