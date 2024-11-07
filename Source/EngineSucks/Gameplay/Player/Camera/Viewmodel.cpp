// Fill out your copyright notice in the Description page of Project Settings.


#include "Viewmodel.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "EngineSucks/Gameplay/Player/CharacterController.h"

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

			SetActorHiddenInGame(false); // Only un-hide the viewmodel once we've actually found the local character controller
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("AViewmodel::Show(); Failed to cast from APawn to ACharacterController (localCharacterController)"));
		}
	}
}

void AViewmodel::Hide() {
	SetActorHiddenInGame(true);
	bViewmodelShown = false;
}

void AViewmodel::Tick(float deltaTime) {
	if ( bViewmodelShown ) {
		Super::Tick(deltaTime);

		// Move the viewmodel to the Local Player's Camera
		if ( IsValid( LocalCharacterController.Get() ) ) {
			UCameraController* localCameraController = LocalCharacterController.Get()->GetCameraController();
			if ( IsValid(localCameraController) ) {
				// Change the view-model's transform to match the camera's transform
				SetActorLocation( localCameraController->GetComponentLocation() );
				SetActorRotation( localCameraController->GetComponentRotation() );
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("LocalCharacterController is stale, cannot changes viewmodel's transform."));
		}
	}
}

