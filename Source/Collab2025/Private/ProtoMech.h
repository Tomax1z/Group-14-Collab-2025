// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "ProtoMech.generated.h"

class UBoxComponent;

UCLASS()
class COLLAB2025_API AProtoMech : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProtoMech();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Paramaters")
	float _MoveSpeed;
	UPROPERTY(EditAnywhere, Category="Movement Paramaters")
	bool _Loop;
	UPROPERTY()
	float _SplineLength;

	UPROPERTY()
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _MechMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> _MechCollision;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> _MechSpline;

	UFUNCTION()
	void MoveMech(float influence);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
