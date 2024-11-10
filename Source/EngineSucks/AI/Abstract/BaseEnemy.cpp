// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "EnemyHealth.h"

ABaseEnemy::ABaseEnemy() {
	PrimaryActorTick.bCanEverTick = false;

	Health = CreateDefaultSubobject<UEnemyHealth>("EnemyHealth");
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
