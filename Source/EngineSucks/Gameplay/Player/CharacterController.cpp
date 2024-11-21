// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

#include "Blueprint/UserWidget.h"
#include "Components/PlayerHealth.h"
#include "EngineSucks/AI/Abstract/BaseEnemy.h"
#include "EngineSucks/AI/Abstract/EnemyHealth.h"
#include "EngineSucks/Gameplay/Weapons/Shotgun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ACharacterController::ACharacterController()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraController = CreateDefaultSubobject<UCameraController>("CameraComponent");
	CameraController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	CameraController->SetRelativeLocation(FVector::ZAxisVector * 60.f);
	
	Health = CreateDefaultSubobject<UPlayerHealth>("Health");
	
	RemainingMidAirJumps = TotalMidAirJumps;
}

void ACharacterController::Tick(float deltaTime) {
	Super::Tick(deltaTime);
	
	StepMovementThisFrame(deltaTime);
	StepJumpingThisFrame(deltaTime);
}

void ACharacterController::SetupPlayerInputComponent(UInputComponent* inputComponent) {
	Super::SetupPlayerInputComponent(inputComponent);

	// Movement inputs
	inputComponent->BindAxis("MoveForward", this, &ACharacterController::InputMoveForward);
	inputComponent->BindAxis("MoveRight", this, &ACharacterController::InputMoveRight);

	// Looking inputs
	inputComponent->BindAxis("LookHorizontal", this, &ACharacterController::InputLookHorizontal);
	inputComponent->BindAxis("LookVertical", this, &ACharacterController::InputLookVertical);

	// Jumping inputs
	inputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterController::InputBeginJump);
	inputComponent->BindAction("Jump", IE_Released, this, &ACharacterController::InputStopJump);

	// Primary Use inputs
	inputComponent->BindAction("PrimaryUse", IE_Pressed, this, &ACharacterController::InputBeginPrimaryUse);
	inputComponent->BindAction("PrimaryUse", IE_Released, this, &ACharacterController::InputStopPrimaryUse);

	// Glory kill inputs
	inputComponent->BindAction("GloryKill", IE_Pressed, this, &ACharacterController::InputBeginGloryKill);
}

void ACharacterController::StepMovementThisFrame(float deltaTime) {
	// Make sure the camera controller exists
	if ( IsValid(CameraController) ) {
		// Gather the TTuple containing both the Forward
		// and Right direction used for calculating movement
		TTuple<FVector, FVector> directions = CameraController->GetFlattenedDirectionVectors();
		FVector& forward = directions.Get<0>();
		FVector& right = directions.Get<1>();

		// Calculate the move direction
		// using the analog movement
		FVector moveDirection = forward * AnalogMoveDirection.Y + right * AnalogMoveDirection.X;
		
		// Move this character in the move direction
		AddMovementInput(moveDirection);
	}
}

void ACharacterController::StepJumpingThisFrame(float deltaTime) {
	UCharacterMovementComponent* characterMovement = GetCharacterMovement();
	if ( characterMovement == nullptr ) {
		UE_LOG(LogTemp, Warning, TEXT("GetCharacterMovement(); returned nullptr"));
		return;
	}

	if ( bJumpInputDown && RemainingMidAirJumps > 0 ) {
		if ( !characterMovement->IsMovingOnGround() ) {
			// If the character controller is not on the ground and it's trying to jump
			// take away a remaining mid air jump.
			// Grounded jumps should not count as mid-air jumps
			RemainingMidAirJumps--;
		}

		bJumpInputDown = false; // Force the jump input to be released as we do not want to accidentally use both jumps instantly
		
		Jump();
	}
}

void ACharacterController::BeginPlay() {
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if ( !IsValid(world) ) { return; }
	
	// Create crosshair user interface
	if ( IsValid( CrosshairWidgetReference.Get() ) ) {
		UUserWidget* crosshairWidget = CreateWidget<UUserWidget>(world, CrosshairWidgetReference.Get());
		if ( IsValid(crosshairWidget) ) {
			crosshairWidget->AddToViewport();
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed to create Crosshair Widget"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing CrosshairWidgetReference"));
	}
}

void ACharacterController::Jump() {
	UCharacterMovementComponent* characterMovement = GetCharacterMovement();
	if ( characterMovement ) {
		LaunchCharacter(FVector::ZAxisVector * characterMovement->JumpZVelocity, false, true);
	}
}

void ACharacterController::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);

	RemainingMidAirJumps = TotalMidAirJumps;
}

void ACharacterController::InputMoveForward(float axis) {
	AnalogMoveDirection.Y = axis;
}

void ACharacterController::InputMoveRight(float axis) {
	AnalogMoveDirection.X = axis;
}

void ACharacterController::InputLookVertical(float axis) {
	if ( IsValid(CameraController) ) {
		CameraController->RotateCameraUsingConstraints(FRotator(axis*MouseSensitivity, 0.f, 0.f));
		AnalogCameraRotation.X = axis*MouseSensitivity;
	}
}

void ACharacterController::InputLookHorizontal(float axis) {
	if ( IsValid(CameraController) ) {
		CameraController->RotateCameraUsingConstraints(FRotator(0.f, axis*MouseSensitivity, 0.f));
		AnalogCameraRotation.Y = axis*MouseSensitivity;
	}
}

void ACharacterController::InputBeginPrimaryUse() {
	UWorld* world = GetWorld();

	// because the shotgun had to be created using blueprints because of engine issues
	// find the shotgun the call fire on that shotgun
	UShotgun* shotgun = FindComponentByClass<UShotgun>();
	if ( IsValid(shotgun) ) {
		shotgun->Fire( world );
	}
}

void ACharacterController::InputStopPrimaryUse() {
	
}
 
void ACharacterController::InputBeginGloryKill() {
	PerformGloryKill();
}

void ACharacterController::PerformGloryKill() {
	// TODO!
	// In the future rework how we detect which enemy to perform a glory kill on
	// As right now I intend the game to be played using a controller, and it's difficult to perfectly aim your
	// center of the camera on the enemy you want to glory kill
	// Instead, i should find the closest enemy that's on the screen that can be glory killed
	
	UWorld* world = GetWorld();
	if ( !IsValid(world) ) { return; }
	
	if ( !IsValid(CameraController) ) { return; }

	APlayerController* localPlayerController = Cast<APlayerController>(GetController());
	if ( !IsValid( localPlayerController ) ) { return; }
	
	FVector cameraOrigin = CameraController->GetComponentLocation();

	TArray<ABaseEnemy*> enemiesOnScreen;

	// I know this is unoptimized
	// but... this is better for the player.
	// first iterate through all actors inside of the world
	// every actor which is a classof ABaseEnemy shall go through some edge cases
	// if all edge cases are met, then add that enemy to the enemiesOnScreen array
	for ( TObjectIterator<ABaseEnemy> iterator; iterator; ++iterator ) {
		ABaseEnemy* enemy = *iterator;
		if ( enemy != nullptr ) {
			if ( !IsValid( enemy->Health ) ) { continue; }
			
			FVector enemyWorldPosition = enemy->GetActorLocation();
			float distanceFromPlayer = FVector::Distance( enemyWorldPosition, enemy->GetActorLocation() );
			if ( distanceFromPlayer > 100.f ) {
				continue;
			}
			
			FVector2D screenPosition;
			bool onScreen = UGameplayStatics::ProjectWorldToScreen( localPlayerController, enemyWorldPosition, screenPosition );
			if ( onScreen ) {
				enemiesOnScreen.Add( enemy );
			}
		}
	}
	
	// go through all enemies on screen, the first enemy which can be glory killed shall be glory killed
	for ( ABaseEnemy* enemy : enemiesOnScreen ) {
		bool success = enemy->Health->TryPerformGloryKill( this );
		if ( success ) {
			break;
		}
	}
}
