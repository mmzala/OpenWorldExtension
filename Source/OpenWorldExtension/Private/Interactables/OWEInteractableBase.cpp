// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/OWEInteractableBase.h"
#include "Components/SceneComponent.h"

// Sets default values
AOWEInteractableBase::AOWEInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;
}

