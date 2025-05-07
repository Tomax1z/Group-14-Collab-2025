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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TObjectPtr<AActor> _CurrentOxygenPickup = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TObjectPtr<AActor> _CurrentEnergyPickup = nullptr;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup|SpawnPoints", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UArrowComponent> _OxygenSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup|SpawnPoints", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UArrowComponent> _EnergySpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup|Classes")
	TSubclassOf<AActor> _OxygenTankPickupClass;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup|Classes")
	TSubclassOf<AActor> _EnergyCellPickupClass;
	
	UFUNCTION(BlueprintCallable, Category="Pickup|Spawn")
	AActor* SpawnPickupAtArrow(TSubclassOf<AActor> PickupClass, UArrowComponent* SpawnArrow);

	UFUNCTION(BlueprintCallable, Category="Pickup|Spawn")
	void SpawnOxygenPickup();

	UFUNCTION(BlueprintCallable, Category="Pickup|Spawn")
	void SpawnEnergyPickup();
	
	UFUNCTION(BlueprintCallable, Category="Spawn")
	AActor* SpawnActorAtPoint(TSubclassOf<AActor> ActorClass);
	UFUNCTION(BlueprintCallable, Category="Pickup|Manage")
	void DestroyOxygenPickup();
	UFUNCTION(BlueprintCallable, Category="Pickup|Manage")
	void DestroyEnergyPickup();
	UFUNCTION(BlueprintCallable, Category="Player Interaction")
	void MechReplaceOxygen();
	UFUNCTION(BlueprintCallable, Category="Player Interaction")
	void MechReplacePower();
	UFUNCTION()
	void MoveMech(float influence);

	UPROPERTY(blueprintReadWrite, EditAnywhere, Category="Cracks")
	TArray<AActor*> _LeftCracks;
	UPROPERTY(blueprintReadWrite, EditAnywhere, Category="Cracks")
	TArray<AActor*> _RightCracks;
	
	UFUNCTION(BlueprintCallable, Category="Grab Arm")
	void SpawnGrabArm();

	UFUNCTION(BlueprintCallable, Category="Grab Arm")
	void TriggerGrabArm();

	virtual void SetSprintStatus_Implementation(bool bIsSprinting) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
