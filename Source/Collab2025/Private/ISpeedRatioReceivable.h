// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISpeedRatioReceivable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UISpeedRatioReceivable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLAB2025_API IISpeedRatioReceivable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Movement|Speed")
	void SetSpeedRatio(float SprintSpeed, float BaseSpeed);
};
