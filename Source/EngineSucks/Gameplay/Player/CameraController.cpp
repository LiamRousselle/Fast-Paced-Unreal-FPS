// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraController.h"

const FVector XYPlane = FVector(1.f, 1.f, 0.f);

UCameraController::UCameraController() {
	FieldOfView = 110.f;
	
}

TTuple<FVector, FVector> UCameraController::GetFlattenedDirectionVectors() {
	FVector forward = GetForwardVector() * XYPlane;
	FVector right = GetRightVector() * XYPlane;
	return TTuple<FVector, FVector>(forward.GetSafeNormal(), right.GetSafeNormal());
}

void UCameraController::RotateCameraUsingConstraints(FRotator direction) {
	FRotator current = GetRelativeRotation();

	float pitch = FMath::Clamp(direction.Pitch + current.Pitch, -80.f, 80.f);
	float yaw = current.Yaw + direction.Yaw;

	SetRelativeRotation(FRotator(
		pitch,
		yaw,
		0.f
	));
}
