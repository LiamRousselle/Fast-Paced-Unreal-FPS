// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UAICacheSystem;
class AEnemyWaypoint;
class AEnemyAIController;
class UEnemyHealth;

// How often the throttled tick function is fired
const float k_ThrottledTickRate = 1.f/10.f;

UCLASS()
class ENGINESUCKS_API ABaseEnemy : public ACharacter {
	GENERATED_BODY()

public:
	ABaseEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UEnemyHealth* Health;

	AEnemyWaypoint* GetTargetWaypoint() { return TargetWaypoint.Get(); }
	void SetTargetWaypoint(AEnemyWaypoint* waypoint);
	
protected:
	AEnemyAIController* GetAIController();

	TObjectPtr<UAICacheSystem> AICacheSystem;
	
	virtual void BeginPlay() override;
	virtual void ThrottledTick() {}

	virtual void MoveTo(
		FVector location,
		float acceptanceRadius = -1,
		bool stopOnOverlap = true,
		bool usePathfinding = true,
		bool navigation = false,
		bool canStrafe = true
	);
	virtual void StopMovement();
	
	virtual void IndexAICacheSystem();
	
private:
	TObjectPtr<AEnemyAIController> AIController;
	FTimerHandle ThrottledTickTimer;

	TObjectPtr<AEnemyWaypoint> TargetWaypoint;
};
