// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemy.h"

ARangedEnemy::ARangedEnemy() {
	PrimaryActorTick.bCanEverTick = true;
}

void ARangedEnemy::BeginPlay() {
	Super::BeginPlay();
	
}

void ARangedEnemy::Tick(float deltaTime) {
	Super::Tick(deltaTime);
}
