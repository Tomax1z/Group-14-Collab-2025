// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Arm.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLLAB2025_API UArm : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArm();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
