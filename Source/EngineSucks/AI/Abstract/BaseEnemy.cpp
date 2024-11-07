// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "EnemyHealth.h"

ABaseEnemy::ABaseEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UEnemyHealth>("EnemyHealth");
}
