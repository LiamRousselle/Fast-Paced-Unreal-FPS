// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UEnemyHealth;

UCLASS()
class ENGINESUCKS_API ABaseEnemy : public ACharacter {
	GENERATED_BODY()

public:
	ABaseEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UEnemyHealth* Health;
};
