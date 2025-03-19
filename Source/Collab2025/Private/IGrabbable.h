// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGrabbable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIGrabbable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLAB2025_API IIGrabbable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Called when the object is grabbed
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	void OnGrabbed(AActor* GrabbingActor);

	// Called when the object is released
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	void OnReleasedGrab(AActor* GrabbingActor);

	// Check if the object can be grabbed
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	bool CanBeGrabbed() const;
};
