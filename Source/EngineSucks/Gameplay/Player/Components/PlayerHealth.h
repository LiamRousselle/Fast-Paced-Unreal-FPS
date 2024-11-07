// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealth.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGINESUCKS_API UPlayerHealth : public UActorComponent {
	GENERATED_BODY()

public:
	UPlayerHealth();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 150.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = MaxHealth;
};
