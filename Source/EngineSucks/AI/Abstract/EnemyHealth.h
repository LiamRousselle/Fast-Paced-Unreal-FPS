// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyHealth.generated.h"

#define ENEMY_HEALTH_TICK_RATE					float(0.1)

// Fired once the health has been stunned
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnStunned );
// Fired once the health has been un-stunned
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnStunEnded );

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

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStunned OnStunnedEvent;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStunEnded OnStunEnded;
	
public:
	float RegenHealth = MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cache")
	bool bIsStunned = false;
	
	bool bIsDead = false;
	
public:
	virtual void BeginPlay() override;
	virtual void HealthTick();
	
	void TakeDamage(float damage);
	bool TryPerformGloryKill(ACharacterController* characterController);

protected:
	FTimerHandle TickTimerHandle;
	FTimerHandle GloryKillFinishedTimer;
	FTimerHandle DeathTimerDelay;
	
	float BeginRegenExpirationTime = -1.f;
	
protected:
	virtual void OnStunned();
	virtual void StopStunned();

	virtual void OnGloryKillFinished();
	
	virtual void OnDied();

private:
	UPROPERTY()
	TObjectPtr<AGloryKillFacade> ActiveGloryKillFacade;
	
};
