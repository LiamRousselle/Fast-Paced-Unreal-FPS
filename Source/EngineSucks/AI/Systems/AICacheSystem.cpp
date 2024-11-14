// Fill out your copyright notice in the Description page of Project Settings.


#include "AICacheSystem.h"

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

void UAICacheSystem::ThrottleWaypointFinder() {
	APawn* localPawn = GetLocalPlayerPawn();
	if ( !IsValid(localPawn) ) { return; }

}
