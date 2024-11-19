// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "CameraController.generated.h"

const FVector XYPlane = FVector(1.f, 1.f, 0.f);

UCLASS()
class ENGINESUCKS_API UCameraController : public UCameraComponent {
	GENERATED_BODY()

public:
	UCameraController();

public:
	TTuple<FVector, FVector> GetFlattenedDirectionVectors();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void RotateCameraUsingConstraints(FRotator direction);
};
