// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Shotgun.generated.h"


class AShotgunViewmodel;
class ACharacterController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ENGINESUCKS_API UShotgun : public USceneComponent {
	GENERATED_BODY()

public:
	UShotgun();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<AShotgunViewmodel> ShotgunViewmodelReference;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage = 12.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Firing")
	float FireRateSeconds = 0.75f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Firing")
	int32 BulletsPerShot = 12;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Firing")
	float BulletSpread = 5.f;
	
	virtual void Fire(UWorld* world);
	
protected:
	ACharacterController* GetLocalCharacterController();
	
	virtual void BeginPlay() override;
	
private:
	TObjectPtr<ACharacterController> LocalCharacterController = nullptr;

	UPROPERTY()
	TObjectPtr<AShotgunViewmodel> ShotgunViewmodel;
	
	float FireExpirationTime = -1.f;

private:
	void CastSingleBullet(UWorld* world, FVector& origin, FVector& direction);
	void CastAllBullets(UWorld* world);
	
};
