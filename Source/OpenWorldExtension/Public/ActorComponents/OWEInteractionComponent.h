// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Interfaces/OWEInterfaceInteract.h"

#include "OWEInteractionComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup="OpenWorldExtension", editinlinenew, hidecategories=(Object,LOD,Lighting,TextureStreaming), meta=(DisplayName="Interaction Component", BlueprintSpawnableComponent))
class OPENWORLDEXTENSION_API UOWEInteractionComponent : public USphereComponent, public IOWEInterfaceInteract
{
	GENERATED_BODY()

public:
    UOWEInteractionComponent();

    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void Interact();

    UFUNCTION()
    void UpdateBestInteractActor();

    // Action mapping name for interaction
    UPROPERTY(EditAnywhere, Category = "Interaction")
    FName ActionMappingName;

    // How much time it takes to update the timer that checks for best interactable actor
    UPROPERTY(EditAnywhere, Category = "Interaction")
    float InteractActorUpdateTime;

    // Minimal float value between -1 and 1 that determines how much the player needs to face the interactable to interact with it
    UPROPERTY(EditAnywhere, Category = "Interaction")
    float MinimalDotProductForFacingInteractable;

    // Actors that are now interactable
    UPROPERTY(VisibleAnywhere, Category = "Interaction")
    TArray<AActor*> InteractableActors;

    // Interactable actor that can be interacted
    UPROPERTY(VisibleAnywhere, Category = "Interaction")
    AActor* BestInteractActor;

    FTimerHandle UpdateBestInteractActorTimer;

protected:
    virtual void BeginPlay() override;
};
