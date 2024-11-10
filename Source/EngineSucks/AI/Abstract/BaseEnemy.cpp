// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "EnemyAIController.h"
#include "EnemyHealth.h"

ABaseEnemy::ABaseEnemy() {
	PrimaryActorTick.bCanEverTick = false;

	Health = CreateDefaultSubobject<UEnemyHealth>("EnemyHealth");
}

AEnemyAIController* ABaseEnemy::GetAIController() {
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
