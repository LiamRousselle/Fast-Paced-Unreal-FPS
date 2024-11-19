// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GloryKillFacade.generated.h"

class UCameraComponent;

UCLASS()
class ENGINESUCKS_API AGloryKillFacade : public AActor {
	GENERATED_BODY()

public:
	AGloryKillFacade();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float GloryKillDuration = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
};
