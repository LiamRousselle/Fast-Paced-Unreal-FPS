// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemy.h"

#include "EngineSucks/AI/Abstract/EnemyHealth.h"
#include "EngineSucks/AI/Systems/AICacheSystem.h"
#include "EngineSucks/Volumes/EnemyWaypoint.h"

ARangedEnemy::ARangedEnemy() {
	PrimaryActorTick.bCanEverTick = true;
}

void ARangedEnemy::BeginPlay() {
	Super::BeginPlay();

	IndexAICacheSystem();
}

void ARangedEnemy::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if ( IsValid( Health ) && !Health->bIsStunned ) {
		FacePlayer();
	}
}

void ARangedEnemy::ThrottledTick() {
	float deltaTime = k_ThrottledTickRate;
	Super::ThrottledTick();

	// Try and index the AI cache system if it's not already indexed
	if ( !IsValid( AICacheSystem.Get() ) ) {
		IndexAICacheSystem();
		return;
	}
	
	// Make sure the health component exists, otherwise
	// we're going to be in a lot of trouble.
	if ( Health == nullptr ) {
		UE_LOG(LogTemp, Error, TEXT("%s::ThrottledTick(); Health component does not exist"), *GetName());
		return;
	}

	// Slowly tick down the attack expiration time
	AttackExpirationTime = FMath::Max( AttackExpirationTime - deltaTime, 0.f );
	
	// As long as we're not stunned we can determine how
	// the ranged enemy is going to behave
	if ( !Health->bIsStunned ) {
		switch (StateFlag) {
			default:
				StateFlag = k_None;
				break;

			case k_None:
				StateFlag = k_Passive;
				break;
			case k_Passive:
				TickStatePassive(deltaTime);
				break;
		}
	}
	// But if we're stunned then stop everything we're doing and play by the rules,
	// meaning that we should stop moving and do nothing
	else {
		StopMovement();
	}

	PreviousStateFlag = StateFlag;
}

void ARangedEnemy::PerformAttack() {
	UE_LOG( LogTemp, Log, TEXT("Attacking!") );
}

void ARangedEnemy::TickStatePassive(float deltaTime) {
	// If we changed states reset the waypoint
	if ( StateFlag != PreviousStateFlag ) {
		SetTargetWaypoint( nullptr );
	}
	
	// If we don't have a waypoint try and find one
	if ( GetTargetWaypoint() == nullptr ) {
		// Note!
		// AICacheSystem.Get()->GetRandomAvailableWaypoint() may return nullptr
		// If this is the case this if statement will be called again.
		SetTargetWaypoint( AICacheSystem.Get()->GetRandomAvailableWaypoint() );
		return;
	}
	
	// Move to the target waypoint
	FVector waypointLocation = GetTargetWaypoint()->GetActorLocation();
	MoveTo( waypointLocation );
	
	// Check if we reached the waypoint
	if ( FVector::Distance(waypointLocation, GetActorLocation()) <= 50.f ) {
		// If so change waypoints
		SetTargetWaypoint( AICacheSystem.Get()->GetRandomAvailableWaypoint() );
	}

	// Attack slowly
	if ( AttackExpirationTime == 0.f ) {
		AttackExpirationTime = 0.6f;
		PerformAttack();
	}
}
