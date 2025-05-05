// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IConsumable.h"
#include "IGrabbable.h"
#include "IPickUpable.h"
#include "GameFramework/Actor.h"
#include "EnergyCell.generated.h"

UCLASS()
class COLLAB2025_API AEnergyCell : public AActor, public IIConsumable, public IIGrabbable, public IIPickUpable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnergyCell();

	UPROPERTY()
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _Collision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual FName GetConsumableType_Implementation() override;

	virtual void OnPickUp_Implementation(AThePlayerCharacter* Picker) override;
};
