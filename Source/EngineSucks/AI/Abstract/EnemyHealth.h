// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyHealth.generated.h"

#define ENEMY_HEALTH_TICK_RATE					float(0.1)

class AGloryKillFacade;
class ACharacterController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGINESUCKS_API UEnemyHealth : public UActorComponent {
	GENERATED_BODY()

public:
	UEnemyHealth() = default;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float DelayBeforeRegen = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<AGloryKillFacade> GloryKillFacadeReference;
	
public:
	float RegenHealth = MaxHealth;
	bool bIsStunned = false;

public:
	virtual void BeginPlay() override;
	virtual void HealthTick();
	
	void TakeDamage(float damage);
	void TryPerformGloryKill(ACharacterController* characterController);

protected:
	FTimerHandle TickTimerHandle;
	FTimerHandle GloryKillFinishedTimer;
	
	float BeginRegenExpirationTime = -1.f;
	
protected:
	virtual void OnStunned();
	virtual void StopStunned();

	virtual void OnGloryKillFinished();
	
	virtual void OnDied();
	
};
