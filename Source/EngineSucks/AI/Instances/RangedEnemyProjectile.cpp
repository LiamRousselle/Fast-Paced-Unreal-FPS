// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemyProjectile.h"
#include "EngineSucks/AI/Enemies/RangedEnemy.h"

ARangedEnemyProjectile::ARangedEnemyProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &ARangedEnemyProjectile::OnHit);
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	SetRootComponent(CollisionComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 10.f; // Die after 10 seconds
}

void ARangedEnemyProjectile::OnHit(UPrimitiveComponent* hitComp, AActor* other, UPrimitiveComponent* otherComp,
	FVector normalImpulse, const FHitResult& result
)
{
	// Destroy when hit, and damage, but right now, do shit haha
	if ( IsValid( other ) && other != this ) {
		UE_LOG(LogTemp, Log, TEXT("%s"), *hitComp->GetOwner()->GetName());
		if ( hitComp->GetOwner() && other->GetClass() != ARangedEnemy::StaticClass() ) {
			Destroy();
		}
	}
}