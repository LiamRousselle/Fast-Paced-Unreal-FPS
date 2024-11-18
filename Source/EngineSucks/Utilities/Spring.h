// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Spring.generated.h"

#define SPRING_ITERATIONS					8.f

UCLASS()
class ENGINESUCKS_API USpring : public UActorComponent
{
	GENERATED_BODY()

public:
	USpring() = default;
	
	void SetProperties(
		float mass = 5.f,
		float force = 50.f,
		float damping = 4.f,
		float speed = 4.f
	)
	{
		Mass = mass;
		Force = force;
		Damping = damping;
		Speed = speed;
	};
	
	void Update(float deltaTime);

	void Shove(FVector direction) { Velocity += direction; };
	
	FVector Target = FVector();
	FVector Position = FVector();
	FVector Velocity = FVector();
	
protected:
	float Mass = 5.f;
	float Force = 50.f;
	float Damping = 4.f;
	float Speed = 4.f;
};
