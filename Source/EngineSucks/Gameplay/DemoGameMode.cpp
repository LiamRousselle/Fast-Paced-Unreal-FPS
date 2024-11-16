// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGameMode.h"
#include "Blueprint/UserWidget.h"

ADemoGameMode::ADemoGameMode() {
	PrimaryActorTick.bCanEverTick = true;
}

void ADemoGameMode::OnEnemyKilled() {
	// Every time an enemy is killed increment our enemy kill count
	EnemyKillCount++;

	// Increase the timer because we killed an enemy
	GameTime = FMath::Min( GameTime + 5.f, MaxGameTime );
}

void ADemoGameMode::BeginPlay() {
	Super::BeginPlay();

	// Creating the UI tracker user interface
	if ( IsValid( UITrackersReference.Get() ) ) {
		UUserWidget* widget = CreateWidget<UUserWidget>( GetWorld(), UITrackersReference.Get() );
		if ( IsValid( widget ) ) {
			widget->AddToViewport();
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing reference for UITrackersReference"));
	}
}

void ADemoGameMode::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	// Tick down the game time every frame
	if ( GameTime > 0.f ) {
		GameTime = FMath::Max( GameTime - deltaTime, 0.f );
	}
}

