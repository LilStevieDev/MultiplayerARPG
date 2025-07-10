// Copyright 2021, Revilian. All Rights Reserved.


#include "TraceUtils.h"
#include <Engine/EngineTypes.h>
#include "PhysicsEngine/PhysicsSettings.h"
#include "KismetTraceUtils.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"



bool UTraceUtils::CapsuleTraceSingle(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);

	static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingleByChannelWithRotation"));
	FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->SweepSingleByChannel(OutHit, Start, End, Orientation.Quaternion(), CollisionChannel, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsuleTraceSingle(World, Start, End, Radius, HalfHeight, Orientation, DrawDebugType, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;
}

bool UTraceUtils::CapsuleTraceMulti(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);

	static const FName CapsuleTraceMultiName(TEXT("CapsuleTraceMultiByChannelWithRotation"));
	FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->SweepMultiByChannel(OutHits, Start, End, Orientation.Quaternion(), CollisionChannel, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsuleTraceMulti(World, Start, End, Radius, HalfHeight, Orientation, DrawDebugType, bHit, OutHits, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;
}

/** For Objects **/

bool UTraceUtils::CapsuleTraceSingleForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingleForObjectsWithRotation"));
	FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	FCollisionObjectQueryParams ObjectParams = ConfigureCollisionObjectParams(ObjectTypes);
	if (ObjectParams.IsValid() == false)
	{
		UE_LOG(LogBlueprintUserMessages, Warning, TEXT("Invalid object types"));
		return false;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->SweepSingleByObjectType(OutHit, Start, End, Orientation.Quaternion(), ObjectParams, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsuleTraceSingle(World, Start, End, Radius, HalfHeight, Orientation, DrawDebugType, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;
}

bool UTraceUtils::CapsuleTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	static const FName CapsuleTraceMultiName(TEXT("CapsuleTraceMultiForObjectsWithRotation"));
	FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	FCollisionObjectQueryParams ObjectParams = ConfigureCollisionObjectParams(ObjectTypes);
	if (ObjectParams.IsValid() == false)
	{
		UE_LOG(LogBlueprintUserMessages, Warning, TEXT("Invalid object types"));
		return false;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->SweepMultiByObjectType(OutHits, Start, End, Orientation.Quaternion(), ObjectParams, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsuleTraceMulti(World, Start, End, Radius, HalfHeight, Orientation, DrawDebugType, bHit, OutHits, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;
}

/** By Profile **/

bool UTraceUtils::CapsuleTraceSingleByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, FName ProfileName, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingleByProfileWithRotation"));
	FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->SweepSingleByProfile(OutHit, Start, End, Orientation.Quaternion(), ProfileName, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsuleTraceSingle(World, Start, End, Radius, HalfHeight, Orientation, DrawDebugType, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;
}


bool UTraceUtils::CapsuleTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, FName ProfileName, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	static const FName CapsuleTraceMultiName(TEXT("CapsuleTraceMultiByProfileWithRotation"));
	FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->SweepMultiByProfile(OutHits, Start, End, Orientation.Quaternion(), ProfileName, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsuleTraceMulti(World, Start, End, Radius, HalfHeight, Orientation, DrawDebugType, bHit, OutHits, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;
}



////////


FCollisionQueryParams UTraceUtils::ConfigureCollisionParams(FName TraceTag, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, const UObject* WorldContextObject)
{
	FCollisionQueryParams Params(TraceTag, SCENE_QUERY_STAT_ONLY(KismetTraceUtils), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.bReturnFaceIndex = !UPhysicsSettings::Get()->bSuppressFaceRemapTable; // Ask for face index, as long as we didn't disable globally
	Params.AddIgnoredActors(ActorsToIgnore);
	if (bIgnoreSelf)
	{
		const AActor* IgnoreActor = Cast<AActor>(WorldContextObject);
		if (IgnoreActor)
		{
			Params.AddIgnoredActor(IgnoreActor);
		}
		else
		{
			// find owner
			const UObject* CurrentObject = WorldContextObject;
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				IgnoreActor = Cast<AActor>(CurrentObject);
				if (IgnoreActor)
				{
					Params.AddIgnoredActor(IgnoreActor);
					break;
				}
			}
		}
	}

	return Params;
}

FCollisionObjectQueryParams UTraceUtils::ConfigureCollisionObjectParams(const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes)
{
	TArray<TEnumAsByte<ECollisionChannel>> CollisionObjectTraces;
	CollisionObjectTraces.AddUninitialized(ObjectTypes.Num());

	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		CollisionObjectTraces[Iter.GetIndex()] = UEngineTypes::ConvertToCollisionChannel(*Iter);
	}

	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = CollisionObjectTraces.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel& Channel = (*Iter);
		if (FCollisionObjectQueryParams::IsValidObjectQuery(Channel))
		{
			ObjectParams.AddObjectTypesToQuery(Channel);
		}
		else
		{
			UE_LOG(LogBlueprintUserMessages, Warning, TEXT("%d isn't valid object type"), (int32)Channel);
		}
	}

	return ObjectParams;
}






