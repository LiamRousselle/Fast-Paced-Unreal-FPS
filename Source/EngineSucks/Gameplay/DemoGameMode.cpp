// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGameMode.h"
#include "Blueprint/UserWidget.h"
#include "EngineSucks/AI/Abstract/BaseEnemy.h"
#include "EngineSucks/AI/Systems/AICacheSystem.h"
#include "EngineSucks/Volumes/EnemyWaypoint.h"
#include "Kismet/GameplayStatics.h"

ADemoGameMode::ADemoGameMode() {
	PrimaryActorTick.bCanEverTick = true;
}

void ADemoGameMode::OnEnemyKilled() {
	// Every time an enemy is killed increment our enemy kill count
	EnemyKillCount++;

	// Decrease the enemies existing count
	EnemiesExisting--;
	
	// Increase the timer because we killed an enemy
	GameTime = FMath::Min( GameTime + 5.f, MaxGameTime );
}

void ADemoGameMode::BeginPlay() {
	Super::BeginPlay();

	// Creating the UI tracker user interface
	if ( IsValid( UITrackersReference.Get() ) ) {
		UITrackers = CreateWidget<UUserWidget>( GetWorld(), UITrackersReference.Get() );
		if ( IsValid( UITrackers.Get() ) ) {
			UITrackers.Get()->AddToViewport();
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing reference for UITrackersReference"));
	}
}

void ADemoGameMode::Tick(float deltaTime) {
	Super::Tick(deltaTime);
	if ( !bGameModeActive ) { return; }

	// Check if the game is over
	if ( GameTime == 0.f ) {
		OnTimeExpired();
		return;
	}

	// Tick down the game time every frame
	if ( GameTime > 0.f ) {
		GameTime = FMath::Max( GameTime - deltaTime, 0.f );
	}

	// If theres not enough enemies in the world spawn some
	if ( EnemiesExisting < EnemySpawnLimit ) {
		SpawnRandomEnemy();
	}
}

void ADemoGameMode::PromptGameOverScreen(UWorld* world) {
	// Remove the old tracker ui from the viewport
	if ( IsValid( UITrackers.Get() ) ) {
		UITrackers.Get()->RemoveFromParent();
	}
	
	// Creating the game over user interface
	if ( IsValid( UIGameOverReference.Get() ) ) {
		UIGameOver = CreateWidget<UUserWidget>( world, UIGameOverReference.Get() );
		if ( IsValid( UIGameOver.Get() ) ) {
			UIGameOver.Get()->AddToViewport();
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing reference for UITrackersReference"));
	}
}

void ADemoGameMode::OnTimeExpired() {
	bGameModeActive = false;

	UE_LOG( LogTemp, Log, TEXT("Expired!") );
	
	UWorld* world = GetWorld();
	if ( IsValid( world ) ) {
		float timeDilation = 0.05f;
		UGameplayStatics::SetGlobalTimeDilation( world, timeDilation );

		PromptGameOverScreen( world );

		// wait 5 seconds before going back to the main menu
		world->GetTimerManager().SetTimer(
			TeleportBackToLobbyDelay,
			this,
			&ADemoGameMode::BackToMainMenu,
			3.f * timeDilation, // Although this seems fast, keep in min Time Dilation is set to 0.05
			false
		);
	}
}

void ADemoGameMode::SpawnRandomEnemy() {
	UWorld* world = GetWorld();
	if ( !IsValid( world ) ) { return; }
	
	if ( EnemySpawnData.Num() == 0 ) {
		UE_LOG(LogTemp, Error, TEXT("Cannot spawn random enemy. EnemySpawnData array is empty!"));
		return;
	}

	AEnemyWaypoint* spawnWaypoint = UAICacheSystem::GetRandomWaypoint( GetWorld() );
	if ( spawnWaypoint ) {
		FEnemySpawnStruct enemySpawnStruct = GetRandomEnemyStructFromSpawnData();
		UE_LOG(LogTemp, Log, TEXT("Spawning enemy %s"), *enemySpawnStruct.EnemyClass.Get()->GetName());

		// spawn in the enemy
		ABaseEnemy* enemy = world->SpawnActor<ABaseEnemy>( enemySpawnStruct.EnemyClass.Get() );
		enemy->SetActorLocation( spawnWaypoint->GetActorLocation() );
		
		EnemiesExisting++;
	}
}

void ADemoGameMode::BackToMainMenu() {
	UE_LOG(LogTemp, Log, TEXT("go back to main menu"));

	// TODO!
	// Change this so you go back to the main menu instead of the testing place!
	UGameplayStatics::OpenLevel( this, "PlayerControllerPrototype" );
}

FEnemySpawnStruct ADemoGameMode::GetRandomEnemyStructFromSpawnData() {
	// First find the max SpawnRarity
	float maxSpawnRarity = 0.f;
	for ( FEnemySpawnStruct enemyStruct : EnemySpawnData ) {
		if ( enemyStruct.SpawnRarity > maxSpawnRarity ) {
			maxSpawnRarity = enemyStruct.SpawnRarity;
		}
	}

	// Pick a number between 0 and the spawn rarity
	float rarityChosen = FMath::RandRange( 0.f, maxSpawnRarity );
	// Any enemy struct spawn rarity that's bigger then the number chosen
	// add it to array
	TArray<FEnemySpawnStruct> availableEnemyStructs;
	for ( FEnemySpawnStruct enemyStruct : EnemySpawnData ) {
		if ( enemyStruct.SpawnRarity >= rarityChosen ) {
			availableEnemyStructs.Add( enemyStruct );
		}
	} 
	
	// Finally pick a random member from the array
	return availableEnemyStructs[FMath::RandRange( 0, availableEnemyStructs.Num() - 1 )];
}
