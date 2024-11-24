// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RangedEnemyProjectile.generated.h"

UCLASS()
class ENGINESUCKS_API ARangedEnemyProjectile : public AActor
{
	GENERATED_BODY()

public:
	ARangedEnemyProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* other, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& result);

private:
	UPROPERTY(VisibleDefaultsOnly, Category="RangedEnemyProjectile")
	USphereComponent* CollisionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RangedEnemyProjectile", meta = (AllowPrivateAccess=true))
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	
};
