// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/OWEInteractionComponent.h"

#include "Interactables/OWEInteractableBase.h"
#include "Kismet/KismetSystemLibrary.h"

UOWEInteractionComponent::UOWEInteractionComponent()
{
    this->OnComponentBeginOverlap.AddDynamic(this, &UOWEInteractionComponent::OnSphereBeginOverlap);
    this->OnComponentEndOverlap.AddDynamic(this, &UOWEInteractionComponent::OnSphereEndOverlap);
}

void UOWEInteractionComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(!OtherActor->IsA(AOWEInteractableBase::StaticClass())) return;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Started overlapping interactable"));
    
    if(!InteractableActors.Contains(OtherActor)) InteractableActors.Add(OtherActor);
}

void UOWEInteractionComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!OtherActor->IsA(AOWEInteractableBase::StaticClass())) return;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Ended overlapping interactable"));

    if(InteractableActors.Contains(OtherActor)) InteractableActors.Remove(OtherActor);
}

void UOWEInteractionComponent::Interact()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Interaction started"));
}

void UOWEInteractionComponent::BeginPlay()
{
    if(GetOwner()->InputComponent && ActionMappingName.IsValid()) GetOwner()->InputComponent->BindAction(ActionMappingName, IE_Pressed, this, &UOWEInteractionComponent::Interact);
    else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No input component found or ActionMappingName not set!"));
}
