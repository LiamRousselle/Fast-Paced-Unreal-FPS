// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Viewmodel.generated.h"

class USpring;
class ACharacterController;
class UArrowComponent;

UCLASS()
class ENGINESUCKS_API AViewmodel : public AActor {
	GENERATED_BODY()

public:
	AViewmodel();
	
	/**
	 * Attaches the viewmodel to the camera, and shows the viewmodel
	 */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel")
	virtual void Show();
	/**
	 * Detaches the viewmodel from the camera, and hides the viewmodel
	 */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel")
	virtual void Hide();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float SwayInfluence = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	bool bSpringJumpingAndLandingAnimations = true;
	
protected:
	virtual void Tick(float deltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Viewmodel", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Springs")
	TObjectPtr<USpring> SwaySpring;
	UPROPERTY(VisibleAnywhere, Category = "Springs")
	TObjectPtr<USpring> MidAirSpring;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditInstanceOnly, Category = "Viewmodel")
	TObjectPtr<UArrowComponent> EditorArrow;
#endif

	TObjectPtr<ACharacterController> LocalCharacterController = nullptr;

private:
	FRotator CurrentMoveTiltRotation = FRotator();
	FRotator PreviousMeshCameraRotation = FRotator(0, 0, 90);
	
	bool bViewmodelShown = false;
	bool bPreviouslyLanded = true;
	
};
