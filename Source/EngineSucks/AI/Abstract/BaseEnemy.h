// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

// How often the throttled tick function is fired
const float k_ThrottledTickRate = 1.f/10.f;

class UEnemyHealth;

UCLASS()
class ENGINESUCKS_API ABaseEnemy : public ACharacter {
	GENERATED_BODY()

public:
	ABaseEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UEnemyHealth* Health;

protected:
	virtual void BeginPlay() override;
	virtual void ThrottledTick() {
		UE_LOG(LogTemp, Log, TEXT("Ticked!"));
	}
	
private:
	FTimerHandle ThrottledTickTimer;
};
