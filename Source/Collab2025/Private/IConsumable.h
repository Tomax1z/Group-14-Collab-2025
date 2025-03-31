// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IConsumable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIConsumable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLAB2025_API IIConsumable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "df")
	FName GetConsumableType();
};
