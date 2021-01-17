// Fill out your copyright notice in the Description page of Project Settings.

/*    TODO:
 *    - Let the user change the class that will be checked if its an interactable, be careful about the interface!
 *    - Same functions as in the interface that you can override in the player character self 
*/

#include "ActorComponents/OWEInteractionComponent.h"

#include "Interactables/OWEInteractableBase.h"
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
    if(!OtherActor->IsA(AOWEInteractableBase::StaticClass())) return;  // If its not an interactable then return
    
    InteractableActors.AddUnique(OtherActor);  // Adding interactable to array

    // If there is a interactable start checking for best interactable
    if(InteractableActors.Num()) GetWorld()->GetTimerManager().SetTimer(UpdateBestInteractActorTimer, this, &UOWEInteractionComponent::UpdateBestInteractActor, InteractActorUpdateTime, true, 0); 
}

void UOWEInteractionComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!OtherActor->IsA(AOWEInteractableBase::StaticClass())) return;  // If its not an interactable then return

    InteractableActors.Remove(OtherActor); // Remove interactable form array

    // If there is no interactable then stop timer and set BestInteractActor to nullptr
    if(!InteractableActors.Num())
    {
        GetWorld()->GetTimerManager().SetTimer(UpdateBestInteractActorTimer, this, &UOWEInteractionComponent::UpdateBestInteractActor, InteractActorUpdateTime, false, 0); 
        BestInteractActor = nullptr;
    }
}

void UOWEInteractionComponent::Interact()
{
    if(!BestInteractActor) return;  // If there is no actor to interact with, then return

    Execute_OnInteract(BestInteractActor, GetOwner());
}

void UOWEInteractionComponent::UpdateBestInteractActor()
{
    static AActor* OldBestInteractActor = nullptr;
    
    // Resetting values
    BestInteractActor = nullptr;
    float LastBestDotProduct = MinimalDotProductForFacingInteractable;

    // Checking with DotProduct which Interactable is the best and setting that interactable as BestInteractActor
    for (AActor* Interactable : InteractableActors)
    {
        FVector LocationDiffrence = Interactable->GetActorLocation() - GetOwner()->GetActorLocation();
        
        float DotProduct = FVector::DotProduct(LocationDiffrence.GetSafeNormal(), GetOwner()->GetActorForwardVector());
        //UE_LOG(LogTemp, Warning, TEXT("%f"),DotProduct);
        
        if(DotProduct > LastBestDotProduct)
        {
            BestInteractActor = Interactable;
            LastBestDotProduct = DotProduct;
        }
    }

    // If BestInteractableActor changed then call OnEndFocus (previous interactable) and OnStartFocus (new interactable)
    if(BestInteractActor != OldBestInteractActor)
    {
        if(OldBestInteractActor) Execute_OnEndFocus(OldBestInteractActor, GetOwner());
        if(BestInteractActor) Execute_OnStartFocus(BestInteractActor, GetOwner());
    }

    //UE_LOG(LogTemp, Warning, TEXT("RUNNING!"));
    
    OldBestInteractActor = BestInteractActor;
}

void UOWEInteractionComponent::BeginPlay()
{   
    if(GetOwner()->InputComponent && ActionMappingName.IsValid()) GetOwner()->InputComponent->BindAction(ActionMappingName, IE_Pressed, this, &UOWEInteractionComponent::Interact);
    else UE_LOG(LogTemp, Warning, TEXT("No input component or no ActionMappingName set!"));
}
