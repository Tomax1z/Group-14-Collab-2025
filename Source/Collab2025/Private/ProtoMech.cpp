// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoMech.h"

#include "Components/BoxComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"


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
	
	
	_MechCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	_MechCollision->SetupAttachment(_MechMesh);
}

// Called when the game starts or when spawned
void AProtoMech::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrabArm();
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
	UE_LOG(LogTemp, Display, TEXT("Spawning Grab Arm"));-
		+
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

