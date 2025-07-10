// Copyright (C) 2019 Grzegorz Szewczyk - All Rights Reserved

#include "CollisionHandler/CCCollisionHandlerComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Net/Core/PushModel/PushModel.h"



FVector FCCCollidingComponent::GetSocketLocation( const FName& SocketName ) const
{
	// if socket is none ( it may happen if component doesn't have any sockets )
	// then use its world location
	if( SocketName.IsNone() )
	{
		return Component->GetComponentLocation();
	}
	else
	{
		return Component->GetSocketLocation( SocketName );
	}
}




// Sets default values for this component's properties
UCCCollisionHandlerComponent::UCCCollisionHandlerComponent()
	: TraceRadius( 0.1f ), TraceCheckInterval( 0.025f )
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	// Enable replication on this component
	SetIsReplicatedByDefault( true );

	// Add EObjectTypeQuery::ObjectTypeQuery3(Pawn) value to objects to collide with
	ObjectTypesToCollideWith.Add( EObjectTypeQuery::ObjectTypeQuery3 );
}

void UCCCollisionHandlerComponent::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const {
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST( UCCCollisionHandlerComponent, bIsCollisionActivated, SharedParams );
	DOREPLIFETIME_WITH_PARAMS_FAST( UCCCollisionHandlerComponent, ActivatedCollisionPart, SharedParams );
}

// Called when the game starts
void UCCCollisionHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCCCollisionHandlerComponent::NotifyOnHit( const FHitResult& hitResult, UPrimitiveComponent* collidingComponent )
{
	// Notify native before blueprint
	OnHitNative.Broadcast( hitResult, collidingComponent );
	OnHit.Broadcast( hitResult, collidingComponent );
}

void UCCCollisionHandlerComponent::NotifyOnCollisionActivated( ECCCollisionPart collisionPart )
{
	// Notify native before blueprint
	OnCollisionActivatedtNative.Broadcast( collisionPart );
	OnCollisionActivated.Broadcast( collisionPart );
}

void UCCCollisionHandlerComponent::NotifyOnCollisionDeactivated()
{
	// Notify native before blueprint
	OnCollisionDeactivatedNative.Broadcast();
	OnCollisionDeactivated.Broadcast();
}

void UCCCollisionHandlerComponent::UpdateSocketLocations()
{
	for( auto& collidingComponent : ActiveCollidingComponents )
	{
		if( IsValid( collidingComponent.Component ) )
		{
			// for each colliding component get its location and store in LastFrameSocketLocations map
			for( const FName& socketName : collidingComponent.Sockets )
			{
				// get socket name
				FVector socketLocation = collidingComponent.GetSocketLocation( socketName );

				// generate unique socket name
				FName uniqueSocketName = GenerateUniqueSocketName( collidingComponent.Component, socketName );

				// store values in map
				LastFrameSocketLocations.Add( uniqueSocketName, socketLocation );
			}
		}
	}
}

void UCCCollisionHandlerComponent::PerformTraceCheck()
{
	for( auto& collidingComponent : ActiveCollidingComponents )
	{
		if( IsValid( collidingComponent.Component ) )
		{
			for( const FName& socketName : collidingComponent.Sockets )
			{
				// generate unique socket name
				FName uniqueSocketName = GenerateUniqueSocketName( collidingComponent.Component, socketName );
				FVector startTrace = *LastFrameSocketLocations.Find( uniqueSocketName );
				FVector endTrace = collidingComponent.GetSocketLocation( socketName );

				// array that will store hit results
				TArray<FHitResult> hitResults;

				// generate array of ignored actors
				TArray<AActor*> ignoredActors{ collidingComponent.HitActors }; // ignore actors that were already hit during this collision window
				ignoredActors.Add( GetOwner() ); // also always ignore owner
				ignoredActors.Append( IgnoredActors ); // ignore default actors ( can be null )

				// do the sphere trace check
				bool wasHit = UKismetSystemLibrary::SphereTraceMultiForObjects( this, startTrace, endTrace, TraceRadius, ObjectTypesToCollideWith,
					bTraceComplex, ignoredActors, EDrawDebugTrace::Type::None, hitResults, true );

				if( wasHit )
				{
					for( const FHitResult& hitResult : hitResults )
					{
						if(AActor* hitActor = hitResult.GetActor())
						{
							// if there was a hit check
							// whether this actor wasn't already hit during this activation
							// whether its class is not ignored
							// whether its profile name is not ignored
							if( collidingComponent.HitActors.Contains( hitActor ) == false &&
								IsIgnoredClass( hitActor->GetClass() ) == false &&
								IsIgnoredProfileName( hitResult.Component->GetCollisionProfileName() ) == false)
							{
								// add to hit actors
								collidingComponent.HitActors.Add( hitActor );

								// call notify
								NotifyOnHit( hitResult, collidingComponent.Component );
#if WITH_EDITOR
								if(bDebug)
								{
									DrawHitSphere( hitResult.Location );
								}
#endif
							}
						}
					}
				}
#if WITH_EDITOR
				if( bDebug )
				{
					DrawDebugTrace( startTrace, endTrace );
				}
#endif
			}
		}
	}
}

/* --------------------------------------------------- DEBUG ------------------------------------------- */

void UCCCollisionHandlerComponent::DrawHitSphere( FVector location )
{
	if( UWorld* world = GetWorld() )
	{
		float radius = TraceRadius >= 8.f ? TraceRadius : 8.f;
		DrawDebugSphere( world, location, radius, 12, FColor::Green, false, 5.f );
	}
}

void UCCCollisionHandlerComponent::DrawDebugTrace( FVector start, FVector end )
{
	if( UWorld* world = GetWorld() )
	{
		DrawDebugCylinder( world, start, end, TraceRadius, 12, FColor::Red, false, 5.f );
	}
}
/* ----------------------------------------------------------------------------------------------------------- */

FName UCCCollisionHandlerComponent::GenerateUniqueSocketName( UPrimitiveComponent* collidingComponent, FName socket )
{
	// concatenate colliding component name + socket to get unique name e.g 'SwordMeshSocket01'
	return FName( *collidingComponent->GetName().Append( socket.ToString() ) );
}

bool UCCCollisionHandlerComponent::IsIgnoredClass( TSubclassOf<AActor> actorClass )
{
	// if actor class is child or same class of any of ignored classes, return true, otherwise false
	for( const auto& ignoredClass : IgnoredClasses )
	{
		if( actorClass->IsChildOf( ignoredClass ) )
			return true;
	}
	return false;
}

bool UCCCollisionHandlerComponent::IsIgnoredProfileName( FName profileName )
{
	// is profile name in array of ignored profile names
	return IgnoredCollisionProfileNames.Contains( profileName );
}

void UCCCollisionHandlerComponent::TraceCheckLoop()
{
	// on first tick just update socket locations so on next tick it will be able to compare socket locations
	if( bCanPerformTrace )
	{
		PerformTraceCheck();
	}

	UpdateSocketLocations();
	bCanPerformTrace = true;

}

void UCCCollisionHandlerComponent::UpdateCollidingComponent( UPrimitiveComponent* component, const TArray<FName>& sockets )
{
	UpdateCollidingComponents( TArray<FCCCollidingComponent>{ FCCCollidingComponent( component, sockets ) } );
}

void UCCCollisionHandlerComponent::UpdateCollidingComponents( const TArray<FCCCollidingComponent>& collidingComponents )
{
	// update CollidingComponents array
	ActiveCollidingComponents = collidingComponents;

	ClearHitActors();
	UpdateSocketLocations();
}

void UCCCollisionHandlerComponent::SetActiveCollisionPart( ECCCollisionPart CollisionPart )
{
	if( ActivatedCollisionPart != CollisionPart )
	{
		ActivatedCollisionPart = CollisionPart;
		MARK_PROPERTY_DIRTY_FROM_NAME( UCCCollisionHandlerComponent, ActivatedCollisionPart, this );
	}
}

void UCCCollisionHandlerComponent::ActivateCollision( ECCCollisionPart collisionPart )
{
	// update active collision part
	SetActiveCollisionPart( collisionPart );

	if( bIsCollisionActivated == false )
	{
		bIsCollisionActivated = true;
		OnRep_IsCollisionActivated();
		MARK_PROPERTY_DIRTY_FROM_NAME( UCCCollisionHandlerComponent, bIsCollisionActivated, this );
	}
}


void UCCCollisionHandlerComponent::DeactivateCollision()
{
	if( bIsCollisionActivated )
	{
		bIsCollisionActivated = false;
		OnRep_IsCollisionActivated();
		MARK_PROPERTY_DIRTY_FROM_NAME( UCCCollisionHandlerComponent, bIsCollisionActivated, this );
	}

}

void UCCCollisionHandlerComponent::OnRep_IsCollisionActivated()
{
	if( bIsCollisionActivated )
	{
		// clear hit actors
		ClearHitActors();

		// notify about collision activation
		NotifyOnCollisionActivated( ActivatedCollisionPart );

		// set timer which will check for collisions
		if( UWorld* world = GetWorld() )
		{
			TraceCheckLoop();
			world->GetTimerManager().SetTimer( TimerHandle_TraceCheck, this, &UCCCollisionHandlerComponent::TraceCheckLoop, TraceCheckInterval, true );
		}
	}
	else
	{
		bCanPerformTrace = false;

		// call notify
		NotifyOnCollisionDeactivated();

		// clear timer checking for collision
		if( UWorld* world = GetWorld() )
		{
			world->GetTimerManager().ClearTimer( TimerHandle_TraceCheck );
		}
	}
}

void UCCCollisionHandlerComponent::ClearHitActors()
{
	// for each colliding component clear HitActors
	for( auto& collidingComponent : ActiveCollidingComponents )
	{
		collidingComponent.HitActors.Empty();
	}
}




