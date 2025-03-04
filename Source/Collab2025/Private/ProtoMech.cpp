// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoMech.h"


// Sets default values
AProtoMech::AProtoMech()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	_MechMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mech Mesh"));
	_MechMesh -> SetupAttachment(_Root);
	_MechSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Mech Spline"));
	_MechSpline -> SetupAttachment(_Root);
}

// Called when the game starts or when spawned
void AProtoMech::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProtoMech::MoveMech(float influence)
{
	float addMechForce = influence * _MoveSpeed;
	float calculateSplineDistance = addMechForce + _SplineLength;
	_SplineLength = calculateSplineDistance;
	
	FTransform mechTransform;
	mechTransform.SetLocation(_MechSpline->GetTransformAtDistanceAlongSpline(_SplineLength, ESplineCoordinateSpace::Local, false).GetLocation());
	mechTransform.SetRotation(_MechSpline->GetTransformAtDistanceAlongSpline(_SplineLength, ESplineCoordinateSpace::Local, false).GetRotation());
	mechTransform.SetScale3D(_MechSpline->GetRelativeScale3D());
	
	_MechMesh->SetRelativeTransform(mechTransform);

	if (_Loop && _SplineLength > _MechSpline->GetSplineLength())
	{
		_SplineLength = 0;
	}
}

// Called every frame
void AProtoMech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveMech(DeltaTime);
}

