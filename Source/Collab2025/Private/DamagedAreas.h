﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagedAreas.generated.h"

UCLASS()
class COLLAB2025_API ADamagedAreas : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamagedAreas();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
