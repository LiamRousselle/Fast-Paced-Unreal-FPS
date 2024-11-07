// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraController.h"
#include "GameFramework/Character.h"
#include "CharacterController.generated.h"

class UPlayerHealth;
class UShotgun;

UCLASS()
class ENGINESUCKS_API ACharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Jump Settings")
	int32 TotalMidAirJumps = 1;
	
public:
	UFUNCTION(BlueprintGetter)
	UCameraController* GetCameraController() { return CameraController; };
	
	virtual void Tick(float deltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	FVector2D AnalogMoveDirection = FVector2D();
	
	bool bJumpInputDown = false;
	int32 RemainingMidAirJumps; // Set within constructor
	
	void StepMovementThisFrame(float deltaTime);
	void StepJumpingThisFrame(float deltaTime);

	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	
	void InputMoveForward(float axis);
	void InputMoveRight(float axis);
	
	void InputLookVertical(float axis);
	void InputLookHorizontal(float axis);

	void InputBeginJump() { bJumpInputDown = true; }
	void InputStopJump() { bJumpInputDown = false; }

	void InputBeginPrimaryUse();
	void InputStopPrimaryUse();

	void InputBeginGloryKill();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = true))
	UCameraController* CameraController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = true))
	UShotgun* Shotgun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = true))
	UPlayerHealth* Health;
	
	void PerformGloryKill();
};
