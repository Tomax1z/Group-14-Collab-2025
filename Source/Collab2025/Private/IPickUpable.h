// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collab2025/PlayerCharacter/ThePlayerCharacter.h"
#include "UObject/Interface.h"
#include "IPickUpable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIPickUpable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLAB2025_API IIPickUpable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PickUp")
	void OnPickUp(AThePlayerCharacter* Picker);
};
