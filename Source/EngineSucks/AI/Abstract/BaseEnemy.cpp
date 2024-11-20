// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "EnemyAIController.h"
#include "EnemyHealth.h"
#include "EngineSucks/AI/Systems/AICacheSystem.h"
#include "EngineSucks/Volumes/EnemyWaypoint.h"

ABaseEnemy::ABaseEnemy() {
	Health = CreateDefaultSubobject<UEnemyHealth>("EnemyHealth");
}

void ABaseEnemy::SetTargetWaypoint(AEnemyWaypoint* waypoint) {
	// If we previously had a Target Waypoint disconnect this enemy from it
	if ( IsValid(TargetWaypoint.Get()) ) {
		TargetWaypoint.Get()->bIsTaken = false;
	}

	TargetWaypoint = waypoint;
}

AEnemyAIController* ABaseEnemy::GetAIController() {
	if ( GetController() == nullptr ) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing AI Controller"), *GetName());
		return nullptr;
	}
	if ( !IsValid( AIController.Get() ) ) {
		AIController = Cast<AEnemyAIController>( GetController() );
	}
	return AIController.Get();
}

void ABaseEnemy::BeginPlay() {
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if ( IsValid(world) ) {
		world->GetTimerManager().SetTimer(
			ThrottledTickTimer,
			this,
			&ABaseEnemy::ThrottledTick,
			k_ThrottledTickRate,
			true
		);
	}

	// Try and index the AI cache system if it's not already indexed
	if ( !IsValid( AICacheSystem.Get() ) ) {
		IndexAICacheSystem();
	}
}

void ABaseEnemy::MoveTo(FVector location, float acceptanceRadius, bool stopOnOverlap, bool usePathfinding, bool navigation, bool canStrafe) {
	if ( !IsValid(AIController.Get()) ) {
		// This looks really weird but i'm going to explain why i'm doing it like this
		// So, basically, AIController may not exist during runtime. Whats going to happen is we're going to
		// see what the result of GetAIController() gets us. If GetAIController() returns nullptr that means
		// that the AIController TObjectPtr is nullptr. Meaning that we failed to find the AIController.
		// But if it succeeds we'll continue through
		if ( !IsValid( GetAIController() ) ) {
			return;
		}
	}
	
	AIController.Get()->MoveToLocation(
		location,
		acceptanceRadius,
		stopOnOverlap,
		usePathfinding,
		navigation,
		canStrafe
	);
}

void ABaseEnemy::StopMovement() {
	if ( !IsValid(AIController.Get()) ) {
		if ( !IsValid( GetAIController() ) ) {
			return;
		}
	}

	AIController.Get()->StopMovement();
}

void ABaseEnemy::IndexAICacheSystem() {
	UGameInstance* instance = GetWorld()->GetGameInstance();
	AICacheSystem = instance ? instance->GetSubsystem<UAICacheSystem>() : nullptr;
}

void ABaseEnemy::FacePlayer() {
	if ( IsValid( AICacheSystem.Get() ) ) {
		APawn* localPlayerPawn = AICacheSystem.Get()->GetLocalPlayerPawn();
		if ( IsValid( localPlayerPawn ) ) {
			FVector faceDirection = ( localPlayerPawn->GetActorLocation() - GetActorLocation() ).GetSafeNormal();
			float faceYawDegress = FMath::RadiansToDegrees( FMath::Atan2( faceDirection.Y, faceDirection.X ) );

			SetActorRotation( FRotator( 0.f, faceYawDegress, 0.f ) );
		}
	}
}
