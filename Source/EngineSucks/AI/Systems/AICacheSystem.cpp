// Fill out your copyright notice in the Description page of Project Settings.


#include "AICacheSystem.h"
#include "EngineUtils.h"
#include "EngineSucks/Volumes/EnemyWaypoint.h"

// Used to flatten a FVector's Z axis
const FVector XYPlane = FVector(1.f, 1.f, 0.f);

APawn* UAICacheSystem::GetLocalPlayerPawn() {
	UWorld* world = GetWorld();
	if ( world ) {
		// We need the player controller to get access to the player controller
		APlayerController* playerController = world->GetFirstPlayerController();
		if ( !IsValid(playerController) ) { return nullptr; }

		return playerController->GetPawn();
	}
	return nullptr;
}

// This function is recursive!
AEnemyWaypoint* UAICacheSystem::GetRandomAvailableWaypoint() {
	if ( AvailableWaypoints.Num() == 0 ) { return nullptr; }
	//if all waypoints are taken then return nullptr
	if ( AllWaypointsOccupied() ) { return nullptr; } 

	// Pick a random waypoint from the available waypoints array
	// If it's taken recursively call this function again
	AEnemyWaypoint* waypoint = AvailableWaypoints[FMath::RandRange(0, AvailableWaypoints.Num() - 1)];
	if ( waypoint->bIsTaken ) {
		return GetRandomAvailableWaypoint();
	}
	
	return waypoint;
}

void UAICacheSystem::Initialize(FSubsystemCollectionBase& collection) {
	Super::Initialize(collection);

	UWorld* world = GetWorld();
	if ( world ) {
		world->GetTimerManager().SetTimer(
			ThrottleWaypointFinderTimer,
			this,
			&UAICacheSystem::ThrottleWaypointFinder,
			k_WaypointFinderTickRate,
			true
		);
	}
}

void UAICacheSystem::Deinitialize() {
	Super::Deinitialize();
	
}

bool UAICacheSystem::AllWaypointsOccupied() {
	for ( AEnemyWaypoint* waypoint : AvailableWaypoints ) {
		if ( !waypoint->bIsTaken ) {
			return false;
		}
	}
	return true;
}

void UAICacheSystem::ThrottleWaypointFinder() {
	UWorld* world = GetWorld();
	
	APawn* localPawn = GetLocalPlayerPawn();
	if ( !IsValid(localPawn) ) { return; }

	FVector localPawnWorldLocationXY = localPawn->GetActorLocation() * XYPlane;
	float localPawnLocationZ = localPawn->GetActorLocation().Z;

	// Clear the previous available waypoints
	AvailableWaypoints.Empty();
	
	// Get all waypoints inside of the world
	// Then do some checks to see if that waypoint is within a certain
	// distance of the localPawn
	for (TActorIterator<AEnemyWaypoint> index( world ); index; ++index) {
		AEnemyWaypoint* waypoint = *index;
		if ( IsValid(waypoint) ) {
			// First check if the local player and waypoint location are in a good radius
			// But don't check using any height, the height check will be done later
			FVector waypointLocationXY = waypoint->GetActorLocation() * XYPlane;
			if ( FVector::Distance(localPawnWorldLocationXY, waypointLocationXY) > k_MaxWaypointValidDistance ) {
				// Exceeds distance check
				continue;
			}

			// now do a height check.
			float waypointLocationZ = waypoint->GetActorLocation().Z;
			float heightDifference = localPawnLocationZ - waypointLocationZ;
			if ( heightDifference > k_WaypointHeightDifferenceLimit || heightDifference < -k_WaypointHeightDifferenceLimit ) {
				// Exceeds height check
				continue;
			}

			// Waypoint passed all tests
			// Add it to the available waypoints array
			AvailableWaypoints.Add( waypoint );
		}
	}
}
