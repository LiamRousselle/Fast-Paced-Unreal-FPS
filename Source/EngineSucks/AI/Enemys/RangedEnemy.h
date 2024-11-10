// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineSucks/AI/Abstract/BaseEnemy.h"
#include "RangedEnemy.generated.h"

UCLASS()
class ENGINESUCKS_API ARangedEnemy : public ABaseEnemy {
	GENERATED_BODY()

public:
	ARangedEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float deltaTime) override;
};
