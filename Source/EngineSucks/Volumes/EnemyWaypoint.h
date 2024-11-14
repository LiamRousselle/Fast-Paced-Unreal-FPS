// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWaypoint.generated.h"

class UCapsuleComponent;

UCLASS()
class ENGINESUCKS_API AEnemyWaypoint : public AActor {
	GENERATED_BODY()

public:
	AEnemyWaypoint();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Waypoint", meta = (AllowPrivateAccess = true))
	UCapsuleComponent* Volume;
	
};
