// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWaypoint.h"
#include "Components/CapsuleComponent.h"


AEnemyWaypoint::AEnemyWaypoint() {
	Volume = CreateDefaultSubobject<UCapsuleComponent>("Volume");
	Volume->SetCapsuleHalfHeight(88.f);
	Volume->SetCapsuleRadius(34.f);
	Volume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Volume->CanCharacterStepUpOn = ECB_No;
	
	SetRootComponent( Volume );
}
