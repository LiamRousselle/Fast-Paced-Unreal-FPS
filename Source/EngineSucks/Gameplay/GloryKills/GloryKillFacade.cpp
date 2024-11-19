// Fill out your copyright notice in the Description page of Project Settings.

#include "GloryKillFacade.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGloryKillFacade::AGloryKillFacade() {
	PrimaryActorTick.bCanEverTick = true;

	// Replace root component with capsule collider
	// but remove the capsule colliders collision
	UCapsuleComponent* root = CreateDefaultSubobject<UCapsuleComponent>("ViewmodelBounds");
	root->SetCapsuleHalfHeight(25.f);
	root->SetCapsuleRadius(25.f);
	root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent( root );

	Camera = CreateDefaultSubobject<UCameraComponent>("GloryKilLCamera");
	Camera->AttachToComponent( root, FAttachmentTransformRules::KeepRelativeTransform );
	Camera->FieldOfView = 45.f;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->AttachToComponent( root, FAttachmentTransformRules::KeepRelativeTransform );
}