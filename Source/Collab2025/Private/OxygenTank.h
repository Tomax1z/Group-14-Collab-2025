// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IConsumable.h"
#include "IGrabbable.h"
#include "GameFramework/Actor.h"
#include "OxygenTank.generated.h"

UCLASS()
class COLLAB2025_API AOxygenTank : public AActor, public IIGrabbable, public IIConsumable
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AOxygenTank();

	UPROPERTY()
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _Collision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual FName GetConsumableType_Implementation() override;
};
