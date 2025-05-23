// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoMech.h"
#include "Components/BoxComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "ISpeedRatioReceivable.h"
#include "Collab2025/PlayerCharacter/ThePlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProtoMech::AProtoMech()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_MechMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mech Mesh"));
	//_MechMesh -> SetupAttachment(_MechSpline);
	
	_MechSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Mech Spline"));
	//_MechSpline -> SetupAttachment(_MechMesh);

	_SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	_SpawnPoint->SetupAttachment(_MechMesh);
	_SpawnPoint->SetArrowColor(FColor::Green);

	_OxygenSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("OxygenSpawnPoint"));
	_OxygenSpawnPoint->SetupAttachment(_MechMesh);
	_OxygenSpawnPoint->SetArrowColor(FColor::Cyan);

	_EnergySpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("EnergySpawnPoint"));
	_EnergySpawnPoint->SetupAttachment(_MechMesh);
	_EnergySpawnPoint->SetArrowColor(FColor::Yellow);
}

// Called when the game starts or when spawned
void AProtoMech::BeginPlay()
{
	Super::BeginPlay();
	_MoveSpeed = _MoveSpeedBase;
	hasEnergyCell = false;
	hasOxygenTank = false;
	SpawnGrabArm();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn && PlayerPawn->GetClass()->ImplementsInterface(UISpeedRatioReceivable::StaticClass()))
	{
		IISpeedRatioReceivable::Execute_SetSpeedRatio(PlayerPawn, _MoveSpeedSprint, _MoveSpeedBase);
		UE_LOG(LogTemp, Log, TEXT("ProtoMech -> Player.SetSpeedRatio(sprint=%f, base=%f)"), _MoveSpeedSprint, _MoveSpeedBase);
	}
}

void AProtoMech::MoveMech(float influence)
{
	//Calculate distance and mech speed
	float addMechForce = influence * _MoveSpeed;
	float calculateSplineDistance = addMechForce + _SplineLength;
	_SplineLength = calculateSplineDistance;

	//Update mech transform 
	FTransform mechTransform;
	mechTransform.SetLocation(_MechSpline->GetTransformAtDistanceAlongSpline(_SplineLength, ESplineCoordinateSpace::Local, false).GetLocation());
	mechTransform.SetRotation(_MechSpline->GetTransformAtDistanceAlongSpline(_SplineLength, ESplineCoordinateSpace::Local, false).GetRotation());
	mechTransform.SetScale3D(_MechMesh->GetRelativeScale3D());

	//Update the relative transform
	_MechMesh->SetRelativeTransform(mechTransform);

	//Loop functionality
	if (_Loop && _SplineLength > _MechSpline->GetSplineLength())
	{
		_SplineLength = 0;
	}
}

// Called every frame
void AProtoMech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //Use Delta time right now for prototyping but later I will use input values instead
	MoveMech(DeltaTime);
}

void AProtoMech::SpawnGrabArm()
{
	UE_LOG(LogTemp, Display, TEXT("Spawning Grab Arm"));
		
	if (_GrabArmClass)
	{
		// Spawn the grab arm
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		_GrabArm = GetWorld()->SpawnActor<AGrabArm>(_GrabArmClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
		// Attach it to the mech
		if (_GrabArm)
		{
			_GrabArm->AttachToComponent(_MechMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UE_LOG(LogTemp, Warning, TEXT("Found Arm"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NOT Found Arm"));
		}
	}
}

void AProtoMech::TriggerGrabArm()
{
	if (_GrabArm)
	{
		_GrabArm->MoveArm();
	}
}

void AProtoMech::SetSprintStatus_Implementation(bool bIsSprinting)
{
	if (!_PowerDown)
	{
		if (bIsSprinting)
		{
			_MoveSpeed = _MoveSpeedSprint;
			UE_LOG(LogTemp, Log, TEXT("ProtoMech speed set to SPRINT: %f"), _MoveSpeed);
		}
		else
		{
			_MoveSpeed = _MoveSpeedBase;
			UE_LOG(LogTemp, Log, TEXT("ProtoMech speed set to BASE: &f"), _MoveSpeed);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ProtoMech speed not set"));
	}
}

AActor* AProtoMech::SpawnPickupAtArrow(TSubclassOf<AActor> PickupClass, UArrowComponent* SpawnArrow)
{
	if (!PickupClass || !SpawnArrow || !GetWorld())
	{
		return nullptr;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnArrow->GetComponentLocation());
	SpawnTransform.SetRotation(SpawnArrow->GetComponentQuat());

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewPickup = GetWorld()->SpawnActor<AActor>(PickupClass, SpawnTransform, Params);
	if (NewPickup)
	{
		TArray<UPrimitiveComponent*> Prims;
		NewPickup->GetComponents<UPrimitiveComponent>(Prims);
		for (auto* Prim : Prims)
		{
			Prim->SetSimulatePhysics(false);
			Prim->SetEnableGravity(false);
		}

		NewPickup->AttachToComponent(SpawnArrow, FAttachmentTransformRules::KeepWorldTransform);
	}

	return NewPickup;
}

void AProtoMech::SpawnOxygenPickup()
{
	SpawnPickupAtArrow(_OxygenTankPickupClass, _OxygenSpawnPoint);
	_CurrentOxygenPickup = SpawnPickupAtArrow(_OxygenTankPickupClass, _OxygenSpawnPoint);
}

void AProtoMech::SpawnEnergyPickup()
{
	SpawnPickupAtArrow(_EnergyCellPickupClass, _EnergySpawnPoint);
	_CurrentEnergyPickup = SpawnPickupAtArrow(_EnergyCellPickupClass, _EnergySpawnPoint);
}

AActor* AProtoMech::SpawnActorAtPoint(TSubclassOf<AActor> ActorClass)
{
	if (!ActorClass || !GetWorld() || !_SpawnPoint)
	{
		return nullptr;
	}

	FTransform spawnTransform;
	spawnTransform.SetLocation(_SpawnPoint->GetComponentLocation());
	spawnTransform.SetRotation(_SpawnPoint->GetComponentQuat());

	FActorSpawnParameters params;
	params.Owner = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, spawnTransform, params);

	TArray<UPrimitiveComponent*> Prims;
	NewActor->GetComponents<UPrimitiveComponent>(Prims);

	for (UPrimitiveComponent* Prim : Prims)
	{
		Prim->SetSimulatePhysics(true);
		Prim->SetEnableGravity(true);
	}

	return NewActor;
}

void AProtoMech::MechReplaceOxygen()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (AThePlayerCharacter* PC = Cast<AThePlayerCharacter>(Pawn))
	{
		PC->PlayerReplaceOxygen();
	}
}

void AProtoMech::MechReplacePower()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (AThePlayerCharacter* PC = Cast<AThePlayerCharacter>(Pawn))
	{
		PC->PlayerReplacePowerCell();
	}
}
void AProtoMech::DestroyOxygenPickup()
{
	if (_CurrentOxygenPickup)
	{
		_CurrentOxygenPickup->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroying Oxygen pickup"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying Oxygen pickup FAIL;"));
	}
	_CurrentOxygenPickup = nullptr;
}
void AProtoMech::DestroyEnergyPickup()
{
	if (_CurrentEnergyPickup)
	{
		_CurrentEnergyPickup->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroying energy pickup"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying energy pickup FAIL"));
	}
	_CurrentEnergyPickup = nullptr;
}
