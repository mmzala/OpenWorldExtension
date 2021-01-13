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

    
    UPROPERTY(EditAnywhere, Category = "Interaction")
    FName ActionMappingName;

    UPROPERTY(VisibleAnywhere, Category = "Interaction")
    TArray<AActor*> InteractableActors;

protected:
    virtual void BeginPlay() override;
};
