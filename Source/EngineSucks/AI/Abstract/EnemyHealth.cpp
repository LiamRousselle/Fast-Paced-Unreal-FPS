
#include "EnemyHealth.h"

#include "EngineSucks/Gameplay/DemoGameMode.h"
#include "EngineSucks/Gameplay/GloryKills/GloryKillFacade.h"
#include "EngineSucks/Gameplay/Player/CharacterController.h"
#include "Kismet/GameplayStatics.h"

void UEnemyHealth::BeginPlay() {
	Super::BeginPlay();

	// Handle health ticking
	GetWorld()->GetTimerManager().SetTimer(
		TickTimerHandle,
		this,
		&UEnemyHealth::HealthTick,
		ENEMY_HEALTH_TICK_RATE,
		true
	);
}

void UEnemyHealth::HealthTick() {
	if ( bIsDead ) { return; }
	
	if ( BeginRegenExpirationTime <= UGameplayStatics::GetRealTimeSeconds( GetWorld() ) ) {
		// If the health regen cooldown is
		// up begin the health regen
		float healthBack = 5 * ENEMY_HEALTH_TICK_RATE;
		Health = FMath::Min( Health + healthBack, RegenHealth );

		// Stop the stun once we begin regen-ing
		StopStunned();
	}
}

void UEnemyHealth::TakeDamage(float damage) {
	// RegenHealth is the max amount of health possible to regen to
	// Everytime this enemy takes damage decrease RegenHealth by the damage
	// Limit the regen health to 10 if the health is above zero
	// Otherwise limit the regen health to 0
	RegenHealth = FMath::Max( RegenHealth - damage * 0.4f, Health == 0.f ? 0.f : 10.f );

	// Damage the health
	Health = FMath::Max( Health - damage, 0.f );
	
	// Everytime we take damage set the expiration timer
	BeginRegenExpirationTime = UGameplayStatics::GetRealTimeSeconds( GetWorld() ) + DelayBeforeRegen;

	if ( RegenHealth > 0.f ) {
		if ( Health  <= 0.f ) {
			OnStunned();
		}
	}
	else {
		OnDied();
	}
}

void UEnemyHealth::TryPerformGloryKill(ACharacterController* characterController) {
	UWorld* world = GetWorld();
	if ( !IsValid( world ) ) { return; }

	APlayerController* localPlayerController = world->GetFirstPlayerController();
	if ( !IsValid( localPlayerController ) ) {
		UE_LOG(LogTemp, Error, TEXT("Cannot perform glory kill, no local player controller!"));
		return;
	}
	
	if ( !bIsStunned ) {
		UE_LOG(LogTemp, Warning, TEXT("Cannot perform glory kill, enemy not stunned yet."));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Performing glory kill"));
	
	if ( IsValid( GloryKillFacadeReference.Get() ) ) {
		FVector worldLocation = GetOwner()->GetActorLocation();

		FVector gloryKillDirection = ((worldLocation - characterController->GetActorLocation()) * FVector(1.f, 1.f, 0.f)).GetSafeNormal();
		FRotator gloryKillRotation = gloryKillDirection.ToOrientationRotator();
		
		// Create the glory kill actor
		ActiveGloryKillFacade = world->SpawnActor<AGloryKillFacade>( GloryKillFacadeReference.Get() );
		ActiveGloryKillFacade->SetActorLocation( GetOwner()->GetActorLocation() + FVector::UpVector * 60.f );
		ActiveGloryKillFacade->SetActorRotation( gloryKillRotation );

		// Hide the character controller
		characterController->SetActorHiddenInGame( true );
		
		// Hide this actor
		GetOwner()->SetActorHiddenInGame( true );
		GetOwner()->SetActorEnableCollision( false );

		// Consider us dead
		bIsDead = true;
		
		// Set the camera's view target to the glory kill's camera
		localPlayerController->SetViewTarget( ActiveGloryKillFacade.Get() );

		// Wait the glory kill duration, then complete the glory kill
		world->GetTimerManager().SetTimer(
			GloryKillFinishedTimer,
			this,
			&UEnemyHealth::OnGloryKillFinished,
			ActiveGloryKillFacade->GloryKillDuration,
			false
		);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing reference to GloryKillFacade blueprint."), *GetOwner()->GetName());
	}
}

void UEnemyHealth::OnStunned() {
	if ( bIsStunned ) { return; }
	bIsStunned = true;

	UE_LOG(LogTemp, Log, TEXT("Begin stun!"));
	
	BeginRegenExpirationTime = UGameplayStatics::GetRealTimeSeconds( GetWorld() );
}

void UEnemyHealth::StopStunned() {
	if ( !bIsStunned ) { return; }
	bIsStunned = false;

	UE_LOG(LogTemp, Log, TEXT("No longer stunned"));
}

void UEnemyHealth::OnGloryKillFinished() {
	UWorld* world = GetWorld();
	if ( !IsValid( world ) ) { return; }
	
	APlayerController* localPlayerController = world->GetFirstPlayerController();
	if ( !IsValid( localPlayerController ) ) {
		UE_LOG(LogTemp, Error, TEXT("Cannot perform glory kill, no local player controller!"));
		return;
	}
	
	localPlayerController->SetViewTargetWithBlend( localPlayerController->GetPawn(), 0.5f, VTBlend_Cubic );

	if ( IsValid( ActiveGloryKillFacade.Get() ) ) {
		ActiveGloryKillFacade.Get()->SetActorHiddenInGame( true );
	}

	world->GetTimerManager().SetTimer(
		DeathTimerDelay,
		this,
		&UEnemyHealth::OnDied,
		1.f,
		false
	);
}

void UEnemyHealth::OnDied() {
	bIsDead = true;

	// If there was a glory kill facade destroy it
	if ( IsValid( ActiveGloryKillFacade.Get() ) ) {
		ActiveGloryKillFacade.Get()->Destroy();
	}
	
	// For now just destroy the actor
	if ( GetOwner() ) {
		GetOwner()->Destroy();
	}

	// Tell the game mode an enemy has died
	ADemoGameMode* gameMode = Cast<ADemoGameMode>(UGameplayStatics::GetGameMode( GetWorld() ));
	if ( IsValid( gameMode ) ) {
		gameMode->OnEnemyKilled();
	}
}
