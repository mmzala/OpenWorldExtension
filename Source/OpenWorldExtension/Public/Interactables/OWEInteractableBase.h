// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OWEInterfaceInteract.h"

#include "OWEInteractableBase.generated.h"

class USceneComponent;

UCLASS()
class OPENWORLDEXTENSION_API AOWEInteractableBase : public AActor, public IOWEInterfaceInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOWEInteractableBase();
	
	UPROPERTY()
	USceneComponent* RootComp;

};
