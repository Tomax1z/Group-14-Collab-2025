﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "IGrabbable.h"
#include "GrabArm.generated.h"


class AThePlayerCharacter;
class AProtoMech;

UCLASS()
class COLLAB2025_API AGrabArm : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrabArm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USceneComponent> _Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UStaticMeshComponent> _ShoulderMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UStaticMeshComponent> _ArmMesh;

	UPROPERTY(EditAnywhere, Category="Movement")
	UCurveFloat* _GrabbingCurve;

	// Timeline for arm movement
	FTimeline _ArmTimeline;

	UFUNCTION()
	void UpdateArmPosition(float Alpha);

	UFUNCTION()
	void OnArmTimelineFinished();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ref")
	AProtoMech* _ProtoMechRef;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void MoveArm();
	
	// Try to grab an object
	UFUNCTION(BlueprintCallable, Category="Grabbing")
	bool TryGrab();

	// Release the currently grabbed object
	UFUNCTION(BlueprintCallable, Category="Grabbing")
	void ReleaseGrabbedObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartLocation;

	UFUNCTION(BlueprintCallable, Category = "Grab System")
	void HandleArmOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						  bool bFromSweep, const FHitResult& SweepResult);


private:
	AThePlayerCharacter* PlayerCharacter;
	UPROPERTY()
	AActor* _GrabbedObject;
    
	bool _bIsGrabbing = false;
    
	// Collision detection function
	UFUNCTION()
	void OnArmOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
