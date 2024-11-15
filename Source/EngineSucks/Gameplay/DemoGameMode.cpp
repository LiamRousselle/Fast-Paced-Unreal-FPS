// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGameMode.h"

void ADemoGameMode::OnEnemyKilled() {
	// Every time an enemy is killed increment our enemy kill count
	EnemyKillCount++;
}

void ADemoGameMode::BeginPlay() {
	Super::BeginPlay();

	
}
