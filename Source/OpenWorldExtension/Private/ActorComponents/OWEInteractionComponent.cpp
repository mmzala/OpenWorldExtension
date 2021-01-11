// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/OWEInteractionComponent.h"



#include "DrawDebugHelpers.h"
#include "Interactables/OWEInteractableBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UOWEInteractionComponent::UOWEInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Setting default variables for interactable object checking
	SphereRadius = 200.0f;

	CheckObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	CheckObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	ActorClassFilter = AOWEInteractableBase::StaticClass();
}

void UOWEInteractionComponent::CheckForInteractables()
{
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), SphereRadius, CheckObjectTypes, ActorClassFilter, ActorsToIgnore, InteractableActors);
	// Optional: Use to have a visual representation of the SphereOverlapActors
	// DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), SphereRadius, 12, FColor::Red, true, 0.1f);
}


// Called when the game starts
void UOWEInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UOWEInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckForInteractables();
}

