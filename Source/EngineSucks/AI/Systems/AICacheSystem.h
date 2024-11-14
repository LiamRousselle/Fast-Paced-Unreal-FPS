// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AICacheSystem.generated.h"

// How often "ThrottleWaypointFinder" will be called
const float k_WaypointFinderTickRate		= 1.f/5.f;

UCLASS()
class ENGINESUCKS_API UAICacheSystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintGetter, Category = "AI Cache System")
	APawn* GetLocalPlayerPawn();
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;
	
private:
	void ThrottleWaypointFinder();
	
private:
	FTimerHandle ThrottleWaypointFinderTimer;
};
