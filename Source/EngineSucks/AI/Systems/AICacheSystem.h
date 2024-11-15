// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AICacheSystem.generated.h"

class AEnemyWaypoint;
// How close the LocalPlayer needs to be to a waypoint in order for the waypoint to be active
const float k_MaxWaypointValidDistance			= 3000.f;

// The min/max height difference a waypoint may have with the LocalPlayer
const float k_WaypointHeightDifferenceLimit		= 100.f;

// How often "ThrottleWaypointFinder" will be called
const float k_WaypointFinderTickRate			= 1.f/5.f;

UCLASS()
class ENGINESUCKS_API UAICacheSystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintGetter, Category = "AI Cache System")
	APawn* GetLocalPlayerPawn();

	AEnemyWaypoint* GetRandomAvailableWaypoint();
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;
	
private:
	bool AllWaypointsOccupied();
	void ThrottleWaypointFinder();
	
private:
	FTimerHandle ThrottleWaypointFinderTimer;

	TArray<AEnemyWaypoint*> AvailableWaypoints;
};
