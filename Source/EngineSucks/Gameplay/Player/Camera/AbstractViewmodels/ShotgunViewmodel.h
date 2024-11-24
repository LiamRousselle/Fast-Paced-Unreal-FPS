// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineSucks/Gameplay/Player/Camera/Viewmodel.h"
#include "GameFramework/Actor.h"
#include "ShotgunViewmodel.generated.h"

UCLASS()
class ENGINESUCKS_API AShotgunViewmodel : public AViewmodel {
	GENERATED_BODY()

public:
	AShotgunViewmodel();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFired();
};
