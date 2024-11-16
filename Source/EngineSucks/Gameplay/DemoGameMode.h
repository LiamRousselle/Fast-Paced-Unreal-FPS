// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoGameMode.generated.h"

class UUserWidget;

UCLASS()
class ENGINESUCKS_API ADemoGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	ADemoGameMode();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracker")
	int32 GameScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracker")
	int32 EnemyKillCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tracker")
	float MaxGameTime = 150.f;
	
	// The user interface which contains an interface for stuff like Time Remaining, Enemies Killed, ect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<UUserWidget> UITrackersReference;

public:
	void OnEnemyKilled();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GameTime = MaxGameTime;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	
private:
	
	
};
