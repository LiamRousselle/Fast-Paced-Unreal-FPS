﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Viewmodel.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "EngineSucks/Gameplay/Player/CharacterController.h"
#include "EngineSucks/Utilities/Spring.h"
#include "GameFramework/CharacterMovementComponent.h"

AViewmodel::AViewmodel() {
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* root = CreateDefaultSubobject<UCapsuleComponent>("ViewmodelBounds");
	root->SetCapsuleHalfHeight(25.f);
	root->SetCapsuleRadius(25.f);
	root->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetRootComponent(root);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);

#if WITH_EDITORONLY_DATA
	EditorArrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if ( EditorArrow )
	{
		EditorArrow->ArrowColor = FColor(150, 200, 255);
		EditorArrow->bTreatAsASprite = true;
		EditorArrow->SetupAttachment(root);
		EditorArrow->bIsScreenSizeScaled = true;
		EditorArrow->SetSimulatePhysics(false);
		EditorArrow->bHiddenInGame = true;
	}
#endif // WITH_EDITORONLY_DATA
	
}

void AViewmodel::Show() {
	// Show the viewmodel actor
	bViewmodelShown = true;

	// Create the sway spring if it doesn't already exist
	if ( !IsValid( SwaySpring.Get() ) ) {
		SwaySpring = NewObject<USpring>( this );
		SwaySpring->SetProperties( 10, 50, 8, 16 );
	}
	// Create the mid-air spring if it doesn't already exist
	if ( !IsValid( MidAirSpring.Get() ) ) {
		MidAirSpring = NewObject<USpring>( this );
		MidAirSpring->SetProperties( 100, 50,  2, 16 );
	}
	
	UWorld* world = GetWorld();
	if ( IsValid( world ) ) {
		// Find the Local Player's pawn, then cast from that pawn to a ACharacterController
		// If anything fails, log it to the console then abort
		APlayerController* localPlayerController = world->GetFirstPlayerController();
		if ( !IsValid( localPlayerController ) ) {
			UE_LOG(LogTemp, Error, TEXT("AViewmodel::Show(); Failed to get APlayerController from world->GetFirstPlayerController()"));
			return;
		}

		APawn* localPlayerPawn = localPlayerController->GetPawn();
		if ( !IsValid( localPlayerPawn ) ) {
			UE_LOG(LogTemp, Error, TEXT("AViewmodel::Show(); Failed to get APawn from localPlayerController->GetPawn()"));
			return;
		}

		ACharacterController* localCharacterController = Cast<ACharacterController>(localPlayerPawn);
		if ( IsValid(localCharacterController) ) {
			// Success!
			LocalCharacterController = localCharacterController;

			SetActorHiddenInGame( false ); // Only un-hide the viewmodel once we've actually found the local character controller
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("AViewmodel::Show(); Failed to cast from APawn to ACharacterController (localCharacterController)"));
		}
	}
}

void AViewmodel::Hide() {
	PreviousMeshCameraRotation = FRotator(0, 0, 90);
	
	SetActorHiddenInGame( true );
	bViewmodelShown = false;
}

void AViewmodel::Tick(float deltaTime) {
	if ( !IsValid( LocalCharacterController.Get() ) ) {
		return;
	}
	if ( LocalCharacterController.Get()->IsHidden() ) {
		SetActorHiddenInGame( true );
		return;
	}
	
	if ( bViewmodelShown ) {
		SetActorHiddenInGame( false );
		Super::Tick( deltaTime );

		// Make camera rotate along with the skeletal meshes camera bone (if it has one)
		if ( IsValid( Mesh.Get() ) ) {
			if ( IsValid( LocalCharacterController.Get() ) ) {
				FTransform worldSocketTransform = Mesh.Get()->GetSocketTransform( "Camera" );
				FTransform componentTransform = Mesh.Get()->GetComponentTransform();
				FTransform localSocketTransform = worldSocketTransform.GetRelativeTransform( componentTransform );

				FRotator cameraRelativeRotation = localSocketTransform.GetRotation().Rotator();
				FRotator rotationDifference = cameraRelativeRotation - PreviousMeshCameraRotation;
				PreviousMeshCameraRotation = cameraRelativeRotation;

				LocalCharacterController.Get()->GetCameraController()->AddRelativeRotation( rotationDifference * 3.5f );
			}
 		}
		
		// Update the sway spring
		if ( IsValid( SwaySpring.Get() ) ) {
			SwaySpring.Get()->Update( deltaTime );
		}
		// Update the mid-air spring
		if ( IsValid( MidAirSpring.Get() ) ) {
			MidAirSpring.Get()->Update( deltaTime );
		}
		
		// Move the viewmodel to the Local Player's Camera
		if ( IsValid( LocalCharacterController.Get() ) ) {
			UCameraController* localCameraController = LocalCharacterController.Get()->GetCameraController();
			if ( IsValid(localCameraController) ) {
				FRotator cameraRotation = localCameraController->GetComponentRotation();
				
				// Change the view-model's transform to match the camera's transform
				SetActorLocation( localCameraController->GetComponentLocation() );
				SetActorRotation( cameraRotation );

				// Handle swaying
				SwaySpring->Shove( FVector(LocalCharacterController.Get()->AnalogCameraRotation, 0.f) * SwayInfluence * 3.f );
				AddActorLocalRotation( FRotator(-SwaySpring->Position.X, -SwaySpring->Position.Y, 0.f) );

				// Rotate the viewmodel in the direction we're moving
				FRotator moveTiltRotation = FRotator( 0.f, 0.f, LocalCharacterController.Get()->AnalogMoveDirection.X ) * 5.f;
				CurrentMoveTiltRotation += (moveTiltRotation - CurrentMoveTiltRotation) * FMath::Min(deltaTime * 5.f, 1.f);
				
				AddActorLocalRotation( CurrentMoveTiltRotation );

				// Rotate & Position the viewmodel depending on the Mid-Air spring
				AddActorLocalOffset( MidAirSpring.Get()->Position );
				AddActorLocalRotation( FRotator(-MidAirSpring->Position.Z * 0.25f, 0.f, 0.f) );
			}

			// Handle landing spring
			UCharacterMovementComponent* movementComponent = LocalCharacterController.Get()->GetCharacterMovement();
			if ( IsValid( movementComponent ) && bSpringJumpingAndLandingAnimations ) {
				bool nowLanded = movementComponent->IsMovingOnGround();
				if ( nowLanded != bPreviouslyLanded ) {
					if ( nowLanded ) {
						// Character has landed
						MidAirSpring->Shove( FVector::UpVector * -10.f );
					} else {
						// Character has jumped
						MidAirSpring->Shove( FVector::UpVector * 12.5f );
					}
				}
				bPreviouslyLanded = nowLanded;
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("LocalCharacterController is stale, cannot changes viewmodel's transform."));
		}
		
	}
}