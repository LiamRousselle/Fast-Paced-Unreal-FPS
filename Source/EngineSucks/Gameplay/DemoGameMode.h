// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoGameMode.generated.h"

UCLASS()
class ENGINESUCKS_API ADemoGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Mode")
	int32 GameScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Mode")
	int32 EnemyKillCount = 0;

public:
	void OnEnemyKilled();
	
protected:
	virtual void BeginPlay() override;
	
};
