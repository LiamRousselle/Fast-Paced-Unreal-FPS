
#include "EnemyHealth.h"
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
	if ( !bIsStunned ) {
		UE_LOG(LogTemp, Warning, TEXT("Cannot perform glory kill, enemy not stunned yet."));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Performing glory kill"));

	// First we're going to destroy the actor
	if ( IsValid(GetOwner()) ) {
		GetOwner()->Destroy();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Glory kill failed. Could not destroy the Actor owning the health component"));
		return;
	}

	// TODO!
	// After the actor is destroyed
	// Play a glory kill animation
	
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

void UEnemyHealth::OnDied() {
	// TODO!
	// Make it so that when the enemy dies
	// Ragdoll the model, or something like that
	
	// For now just destroy the actor
	if ( GetOwner() ) {
		GetOwner()->Destroy();
	}
}
