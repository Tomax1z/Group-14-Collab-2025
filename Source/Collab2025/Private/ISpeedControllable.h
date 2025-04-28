// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISpeedControllable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UISpeedControllable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLAB2025_API IISpeedControllable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Speed Control")
	void SetSprintStatus(bool bIsSprinting);
};
