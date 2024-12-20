// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineSucks/AI/Abstract/BaseEnemy.h"
#include "RangedEnemy.generated.h"

class ARangedEnemyProjectile;

enum RangedEnemyStateFlags {
	k_None,					// This is the default state which the enemy will spawn as

	k_Passive,				// The enemy will move around the player passively, but still throw ranged attacks towards the player
	k_Aggressive,			// Like passive the enemy will patrol around the player but will attack a lot more frequently
	k_Scared,				// The enemy will attack really slow, and will find a waypoint that is far away from the player
};

UCLASS()
class ENGINESUCKS_API ARangedEnemy : public ABaseEnemy {
	GENERATED_BODY()

public:
	ARangedEnemy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "References")
	TSubclassOf<ARangedEnemyProjectile> ProjectileClassReference;
	
public:
	RangedEnemyStateFlags StateFlag = k_None;
	RangedEnemyStateFlags PreviousStateFlag = StateFlag;
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;
	virtual void ThrottledTick() override;

private:
	void PerformAttack();
	
	void TickStatePassive(float deltaTime);
	
	float AttackExpirationTime = -1.f;
};
