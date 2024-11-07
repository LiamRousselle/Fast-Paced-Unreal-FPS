// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Viewmodel.generated.h"

class ACharacterController;
class UArrowComponent;

UCLASS()
class ENGINESUCKS_API AViewmodel : public AActor {
	GENERATED_BODY()

public:
	AViewmodel();
	
	/**
	 * Attaches the viewmodel to the camera, and shows the viewmodel
	 */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel")
	virtual void Show();
	/**
	 * Detaches the viewmodel from the camera, and hides the viewmodel
	 */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel")
	virtual void Hide();

protected:
	virtual void Tick(float deltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Viewmodel", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> Mesh;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditInstanceOnly, Category = "Viewmodel")
	TObjectPtr<UArrowComponent> EditorArrow;
#endif

	TObjectPtr<ACharacterController> LocalCharacterController = nullptr;

private:
	bool bViewmodelShown = false;
	
};
