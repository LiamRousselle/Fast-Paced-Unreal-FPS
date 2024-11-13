// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemy.h"

#include "EngineSucks/AI/Abstract/EnemyHealth.h"

void ARangedEnemy::BeginPlay() {
	Super::BeginPlay();
	
}

void ARangedEnemy::ThrottledTick() {
	float deltaTime = k_ThrottledTickRate;
	Super::ThrottledTick();
	
	// Make sure the health component exists, otherwise
	// we're going to be in a lot of trouble.
	if ( Health == nullptr ) {
		UE_LOG(LogTemp, Error, TEXT("%s::ThrottledTick(); Health component does not exist"), *GetName());
		return;
	}

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
		
	}
}

void ARangedEnemy::TickStatePassive(float deltaTime) {

}
