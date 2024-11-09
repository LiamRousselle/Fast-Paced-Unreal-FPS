// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

#include "EngineSucks/AI/Abstract/EnemyHealth.h"
#include "EngineSucks/Gameplay/Player/CharacterController.h"
#include "EngineSucks/Gameplay/Player/Camera/Viewmodel.h"
#include "Kismet/GameplayStatics.h"

UShotgun::UShotgun() {
	PrimaryComponentTick.bCanEverTick = false;

}

ACharacterController* UShotgun::GetLocalCharacterController() {
	// Me in future if I ever refactor this code
	// Remove all this ugly nesting, (i'm a never nester btw)
	
	AActor* owningActor = GetOwner();
	if ( IsValid(owningActor) ) {
		// Cast to the ACharacterController
		// If the cast fails throw an error in the output
		ACharacterController* characterController = Cast<ACharacterController>(owningActor);
		if ( IsValid(characterController) ) {
			return characterController;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("UShotgun::BeginPlay() couldn't get AActor that owns the shotgun component"));
	}

	return nullptr;
}

void UShotgun::BeginPlay() {
	Super::BeginPlay();

	// Get the ACharacterController actor which owns this component
	LocalCharacterController = GetLocalCharacterController();

	// Create the shotgun viewmodel
	UWorld* world = GetWorld();
	if ( IsValid(ShotgunViewmodelReference.Get()) && IsValid(world) ) {
		ShotgunViewmodel = NewObject<AViewmodel>(world, ShotgunViewmodelReference.Get());
		if ( IsValid(ShotgunViewmodel) ) {
			ShotgunViewmodel->Show();
			UE_LOG(LogTemp, Log, TEXT("Created shotgun viewmodel successfully"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to create AViewmodel for shotgun."));
	}
}

void UShotgun::CastSingleBullet(UWorld* world, FVector& origin, FVector& direction) {
	FVector bulletGoal = origin + (direction * 10000.f);
	
	FHitResult bulletHitResult;
	bool bulletDidCollide = world->LineTraceSingleByChannel(
		bulletHitResult,
		origin,
		bulletGoal,
		ECC_Visibility
	);

	if ( bulletDidCollide ) {
		DrawDebugSphere(world, bulletHitResult.Location, 5.f, 16, FColor::Green, false, FireRateSeconds);
		DrawDebugLine(world, origin, bulletHitResult.Location, FColor::Green, false, FireRateSeconds);

		if ( !IsValid(bulletHitResult.GetActor()) ) {
			return;
		}
		
		UEnemyHealth* enemyHealth = bulletHitResult.GetActor()->FindComponentByClass<UEnemyHealth>();
		if ( IsValid(enemyHealth) ) {
			enemyHealth->TakeDamage( Damage );
		}
	}
	else {
		DrawDebugLine(world, origin, bulletGoal, FColor::Red, false, FireRateSeconds);
	}
	
}

// Heads up!
// Whatever called this function should have already
// checked if LocalCharacterController exists
void UShotgun::CastAllBullets(UWorld* world) {
	UCameraController* localCameraController = LocalCharacterController.Get()->GetCameraController();
	if ( !IsValid(localCameraController) ) {
		UE_LOG(LogTemp, Error, TEXT("UShotgun::CastAllBullets() failed to get Local Player's Camera Controller."));
		return;
	}

	FVector cameraOrigin = localCameraController->GetComponentLocation();
	FVector cameraForward = localCameraController->GetForwardVector();

	for ( int32 index = 0; index < BulletsPerShot; index++ ) {
		FVector spreadVector = FVector(
			FMath::DegreesToRadians(FMath::FRandRange(-BulletSpread, BulletSpread)),
			FMath::DegreesToRadians(FMath::FRandRange(-BulletSpread, BulletSpread)),
			FMath::DegreesToRadians(FMath::FRandRange(-BulletSpread, BulletSpread))
		);

		// To get the new direction the bullet will travel we can simply just take the
		// cameraForward vector & spreadVector vector and add them together
		// However, now we don't have a direction but instead a position, so to turn it back into
		// a direction we can just normalize it
		FVector bulletForward = (cameraForward + spreadVector).GetSafeNormal();
		CastSingleBullet(world, cameraOrigin, bulletForward);
	}
}

void UShotgun::Fire(UWorld* world) {
	// Ensure LocalCharacterController exists, if not throw a warning in the output
	if ( !IsValid(LocalCharacterController.Get()) ) {
		LocalCharacterController = GetLocalCharacterController();
		UE_LOG(LogTemp, Warning, TEXT("LocalCharacterController is stale, cannot fire shotgun"));
		
		return;
	}

	// Prevent the user from firing to quickly by spamming the PrimaryUse input
	// Instead, allow to the shotgun to be fired at a fixed rate (see FireRateSeconds)
	if ( FireExpirationTime > UGameplayStatics::GetRealTimeSeconds(world) ) { return; }
	
	FireExpirationTime = UGameplayStatics::GetRealTimeSeconds( world ) + FireRateSeconds;
	
	CastAllBullets(world);
}
