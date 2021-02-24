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
    MinimalDotProductForFacingInteractable = -0.2f;
}

void UOWEInteractionComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(!OtherActor->GetClass()->ImplementsInterface(UOWEInteractInterface::StaticClass())) return;  // If its not an interactable then return
    
    InteractableActors.AddUnique(OtherActor);  // Adding interactable to array

    // If there is a interactable start checking for best interactable
    if(InteractableActors.Num()) GetWorld()->GetTimerManager().SetTimer(UpdateBestInteractActorTimer, this, &UOWEInteractionComponent::UpdateBestInteractActor, InteractActorUpdateTime, true, 0); 
}

void UOWEInteractionComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!OtherActor->GetClass()->ImplementsInterface(UOWEInteractInterface::StaticClass())) return;  // If its not an interactable then return

    InteractableActors.Remove(OtherActor); // Remove interactable form array

    // If there is no interactable then stop checking for best interactable
    if(!InteractableActors.Num()) GetWorld()->GetTimerManager().SetTimer(UpdateBestInteractActorTimer, this, &UOWEInteractionComponent::UpdateBestInteractActor, InteractActorUpdateTime, false, 0);
}

void UOWEInteractionComponent::Interact()
{
    if(!IsValid(BestInteractActor)) return;  // If there is no actor to interact with, then return

    // Calling OnInteract for interactable
    Execute_OnInteract(BestInteractActor, Owner);
}

void UOWEInteractionComponent::UpdateBestInteractActor()
{
    // Resetting values
    BestInteractActor = nullptr;
    float LastBestDotProduct = MinimalDotProductForFacingInteractable;

    // Checking with DotProduct which Interactable is the best and setting that interactable as BestInteractActor
    for (AActor* Interactable : InteractableActors)
    {
        const FVector InteractableVector = Interactable->GetActorLocation() - Owner->GetActorLocation();
        
        const float DotProduct = FVector::DotProduct(InteractableVector.GetSafeNormal(), Owner->GetActorForwardVector());
        
        if(DotProduct < LastBestDotProduct) continue;
        
        BestInteractActor = Interactable;
        LastBestDotProduct = DotProduct;
    }

    // If BestInteractableActor changed then call OnEndFocus and OnStartFocus for interactables
    if(BestInteractActor != PrevBestInteractActor) CallFocusEvents();
    
    PrevBestInteractActor = BestInteractActor;
}

void UOWEInteractionComponent::CallFocusEvents() const
{
    if(IsValid(PrevBestInteractActor)) Execute_OnEndFocus(PrevBestInteractActor, Owner);
    if(IsValid(BestInteractActor)) Execute_OnStartFocus(BestInteractActor, Owner);
}

void UOWEInteractionComponent::BeginPlay()
{
    Owner = GetOwner();
    
    // Set up input for interaction
    if(Owner->InputComponent && ActionMappingName.IsValid()) Owner->InputComponent->BindAction(ActionMappingName, IE_Pressed, this, &UOWEInteractionComponent::Interact);
    else UE_LOG(LogTemp, Warning, TEXT("No input component or no ActionMappingName set!"));
}
