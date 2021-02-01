#pragma once

#include "CoreMinimal.h"

#include "OWEInteractionComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/OWEInteractInterface.h"

#include "OWEInteractionComponent.generated.h"

// Delegates for interaction events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnStartFocusSignature, AActor*, Interactable );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnInteractSignature, AActor*, Interactable );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnEndFocusSignature, AActor*, Interactable );


UCLASS(ClassGroup="OpenWorldExtension", editinlinenew, hidecategories=(Object,LOD,Lighting,TextureStreaming), meta=(DisplayName="Interaction Component", BlueprintSpawnableComponent))
class OPENWORLDEXTENSION_API UOWEInteractionComponent : public USphereComponent, public IOWEInteractInterface
{
	GENERATED_BODY()

public:
    UOWEInteractionComponent();

    // Overlap events
    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Calls interact events 
    void Interact();

    // Updates BestInteractActor
    void UpdateBestInteractActor();

    // Calls focus events
    void CallFocusEvents() const;

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

    UPROPERTY()
    AActor* PrevBestInteractActor;

    UPROPERTY()
    AActor* Owner;

    // Event called when started focusing on an interactable
    UPROPERTY(BlueprintAssignable, Category="Interaction")
    FOnStartFocusSignature OnStartFocus;

    // Event called when interacted
    UPROPERTY(BlueprintAssignable, Category="Interaction")
    FOnInteractSignature OnInteract;

    // Event called when ended focusing on an interactable
    UPROPERTY(BlueprintAssignable, Category="Interaction")
    FOnEndFocusSignature OnEndFocus;

    // Timer for updating BestInteractActor
    FTimerHandle UpdateBestInteractActorTimer;
    
protected:
    virtual void BeginPlay() override;
};
