// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoGameMode.generated.h"

class ABaseEnemy;
class UUserWidget;

USTRUCT(BlueprintType)
struct FEnemySpawnStruct {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABaseEnemy> EnemyClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SpawnRarity = 100.f;
};

UCLASS()
class ENGINESUCKS_API ADemoGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	ADemoGameMode();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracker")
	int32 GameScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracker")
	int32 EnemyKillCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	int32 EnemySpawnLimit = 9;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float MaxGameTime = 150.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float GameTime = 150.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Spawns")
	TArray<FEnemySpawnStruct> EnemySpawnData;
	
	// The user interface which contains an interface for stuff like Time Remaining, Enemies Killed, ect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<UUserWidget> UITrackersReference;

	// The user interface displayed when the game is over!
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<UUserWidget> UIGameOverReference;
	
public:
	void OnEnemyKilled();

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> UITrackers;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> UIGameOver;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	void PromptGameOverScreen(UWorld* world);
	void OnTimeExpired();
	
	void SpawnRandomEnemy();

	void BackToMainMenu();
	
private:
	bool bGameModeActive = true;
	
	// The enemies that have been spawned in the world by this game mode
	int32 EnemiesExisting = 0;

	FTimerHandle TeleportBackToLobbyDelay;
	
	FEnemySpawnStruct GetRandomEnemyStructFromSpawnData();
};
