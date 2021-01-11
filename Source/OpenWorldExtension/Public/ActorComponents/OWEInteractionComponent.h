// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OWEInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENWORLDEXTENSION_API UOWEInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOWEInteractionComponent();
	
	void CheckForInteractables();

	// Radius of sphere that checks for interactable objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractableChecking")
	float SphereRadius;

	// Object types that the collider will check for when searching for interactable objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractableChecking")
	TArray<TEnumAsByte<EObjectTypeQuery>> CheckObjectTypes;

	// Specific actor filter
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractableChecking")
	UClass* ActorClassFilter;

	// What actors to ignore when checking for interactable objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractableChecking")
	TArray<AActor*> ActorsToIgnore;

	// Interactable actors that are in the collider
	UPROPERTY(VisibleAnywhere, Category = "InteractableChecking")
	TArray<AActor*> InteractableActors;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
