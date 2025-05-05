// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "GrabArm.h"
#include "ISpeedControllable.h"
#include "ProtoMech.generated.h"

class UBoxComponent;

UCLASS()
class COLLAB2025_API AProtoMech : public AActor, public IISpeedControllable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProtoMech();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Paramaters")
	float _MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Paramaters")
	float _MoveSpeedBase = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Paramaters")
	float _MoveSpeedSprint = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Paramaters")
	bool _PowerDown = false;
	UPROPERTY(EditAnywhere, Category="Movement Paramaters")
	bool _Loop;
	UPROPERTY()
	float _SplineLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawn")
	TObjectPtr<UArrowComponent> _SpawnPoint;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _MechMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USplineComponent> _MechSpline;

	UPROPERTY(EditAnywhere, Category="Grab Arm")
	TSubclassOf<AGrabArm> _GrabArmClass;

	UPROPERTY()
	TObjectPtr<AGrabArm> _GrabArm;
	

	UFUNCTION(BlueprintCallable, Category="Spawn")
	AActor* SpawnActorAtPoint(TSubclassOf<AActor> ActorClass);
	
	UFUNCTION()
	void MoveMech(float influence);

	UFUNCTION(BlueprintCallable, Category="Grab Arm")
	void SpawnGrabArm();

	UFUNCTION(BlueprintCallable, Category="Grab Arm")
	void TriggerGrabArm();

	virtual void SetSprintStatus_Implementation(bool bIsSprinting) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
