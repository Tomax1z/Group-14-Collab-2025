// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGrabbable.generated.h"

UINTERFACE(MinimalAPI)
class UIGrabbable : public UInterface
{
	GENERATED_BODY()
};

class COLLAB2025_API IIGrabbable
{
	GENERATED_BODY()

public:
	// Called when object is grabbed
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	void OnGrabbed(AActor* Grabber);
    
	// Called when object is released
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	void OnReleased(AActor* Grabber);
    
	// Tells if this object can be grabbed
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	bool CanBeGrabbed() const;
    
	// Get the type of consumable
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	FString GetConsumableType() const;
};
