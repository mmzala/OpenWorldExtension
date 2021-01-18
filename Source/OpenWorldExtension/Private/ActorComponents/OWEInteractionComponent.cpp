// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponents/OWEInteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UOWEInteractionComponent::UOWEInteractionComponent()
{
    // Adding overlap functions
    this->OnComponentBeginOverlap.AddDynamic(this, &UOWEInteractionComponent::OnSphereBeginOverlap);
    this->OnComponentEndOverlap.AddDynamic(this, &UOWEInteractionComponent::OnSphereEndOverlap);

    // Setting default values
    SphereRadius = 150.0f;
    ActionMappingName = "Interact";
    InteractActorUpdateTime = 0.1f;
    MinimalDotProductForFacingInteractable = -0.3f;
}

void UOWEInteractionComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(!OtherActor->GetClass()->ImplementsInterface(UOWEInterfaceInteract::StaticClass())) return;  // If its not an interactable then return
    
    InteractableActors.AddUnique(OtherActor);  // Adding interactable to array

    // If there is a interactable start checking for best interactable
    if(InteractableActors.Num()) GetWorld()->GetTimerManager().SetTimer(UpdateBestInteractActorTimer, this, &UOWEInteractionComponent::UpdateBestInteractActor, InteractActorUpdateTime, true, 0); 
}

void UOWEInteractionComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!OtherActor->GetClass()->ImplementsInterface(UOWEInterfaceInteract::StaticClass())) return;  // If its not an interactable then return

    InteractableActors.Remove(OtherActor); // Remove interactable form array

    // If there is no interactable then stop timer
    if(!InteractableActors.Num()) GetWorld()->GetTimerManager().SetTimer(UpdateBestInteractActorTimer, this, &UOWEInteractionComponent::UpdateBestInteractActor, InteractActorUpdateTime, false, 0);
}

void UOWEInteractionComponent::Interact()
{
    if(!BestInteractActor) return;  // If there is no actor to interact with, then return

    // Calling OnInteract for interactable and player
    OnInteract.Broadcast();
    Execute_OnInteract(BestInteractActor, Owner);
}

void UOWEInteractionComponent::UpdateBestInteractActor()
{
    static AActor* PrevBestInteractActor = nullptr;
    
    // Resetting values
    BestInteractActor = nullptr;
    float LastBestDotProduct = MinimalDotProductForFacingInteractable;

    // Checking with DotProduct which Interactable is the best and setting that interactable as BestInteractActor
    for (AActor* Interactable : InteractableActors)
    {
        FVector LocationDiffrence = Interactable->GetActorLocation() - Owner->GetActorLocation();
        
        float DotProduct = FVector::DotProduct(LocationDiffrence.GetSafeNormal(), Owner->GetActorForwardVector());
        
        if(DotProduct > LastBestDotProduct)
        {
            BestInteractActor = Interactable;
            LastBestDotProduct = DotProduct;
        }
    }

    // If BestInteractableActor changed then call OnEndFocus and OnStartFocus for interactables and owner
    if(BestInteractActor != PrevBestInteractActor)
    {
        if(PrevBestInteractActor)
        {
            OnEndFocus.Broadcast();
            Execute_OnEndFocus(PrevBestInteractActor, Owner);
        }
        if(BestInteractActor)
        {
            OnStartFocus.Broadcast();
            Execute_OnStartFocus(BestInteractActor, Owner);
        }
    }
    
    PrevBestInteractActor = BestInteractActor;
}

void UOWEInteractionComponent::BeginPlay()
{
    Owner = GetOwner();
    
    // Set up input for interaction
    if(Owner->InputComponent && ActionMappingName.IsValid()) Owner->InputComponent->BindAction(ActionMappingName, IE_Pressed, this, &UOWEInteractionComponent::Interact);
    else UE_LOG(LogTemp, Warning, TEXT("No input component or no ActionMappingName set!"));
}
