// Fill out your copyright notice in the Description page of Project Settings.


#include "Spring.h"

void USpring::Update(float deltaTime)
{
	float scaledDeltaTime = FMath::Min(deltaTime, 1.f) * Speed / SPRING_ITERATIONS;
	
	for (int32 i = 0; i < SPRING_ITERATIONS; i++)
	{
		FVector iterationForce = Target - Position;
		FVector acceleration = (iterationForce * Force) / Mass;
		acceleration -= Velocity * Damping;

		Velocity += acceleration * scaledDeltaTime;
		Position += Velocity * scaledDeltaTime;
	}
}
