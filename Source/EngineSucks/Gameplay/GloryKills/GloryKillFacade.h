// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GloryKillFacade.generated.h"

UCLASS()
class ENGINESUCKS_API AGloryKillFacade : public AActor {
	GENERATED_BODY()

public:
	AGloryKillFacade();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
};
