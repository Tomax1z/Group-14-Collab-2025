// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabArm.h"

#include "ProtoMech.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGrabArm::AGrabArm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = _Root;

	_ArmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arm Mesh"));
	_ArmMesh->SetupAttachment(_Root);

	_ArmCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Arm Collision"));
	_ArmCollision->SetupAttachment(_ArmMesh);

	// Initialize the target location
}

// Called when the game starts or when spawned
void AGrabArm::BeginPlay()
{
	Super::BeginPlay();

	// Setup timeline if we have a curve
	if (_GrabbingCurve)
	{
		FOnTimelineFloat timelineCallback;
		timelineCallback.BindUFunction(this, FName("UpdateArmPosition"));

		FOnTimelineEvent timelineFinishedCallback;
		timelineFinishedCallback.BindUFunction(this, FName("OnArmTimelineFinished"));

		_ArmTimeline.AddInterpFloat(_GrabbingCurve, timelineCallback);
		_ArmTimeline.SetTimelineFinishedFunc(timelineFinishedCallback);
	}
	else
	{
		// Create a default curve if none is provided
		UCurveFloat* Curve = NewObject<UCurveFloat>(this);

		// Add the key points from your timeline in the Blueprint
		Curve->FloatCurve.AddKey(0.0f, 0.0f);
		Curve->FloatCurve.AddKey(1.0f, 1.0f);
		Curve->FloatCurve.AddKey(2.0f, 0.0f);

		_GrabbingCurve = Curve;
        
		FOnTimelineFloat timelineCallback;
		timelineCallback.BindUFunction(this, FName("UpdateArmPosition"));
        
		FOnTimelineEvent timelineFinishedCallback;
		timelineFinishedCallback.BindUFunction(this, FName("OnArmTimelineFinished"));
        
		_ArmTimeline.AddInterpFloat(_GrabbingCurve, timelineCallback);
		_ArmTimeline.SetTimelineFinishedFunc(timelineFinishedCallback);
	}

	AActor* MechRef = UGameplayStatics::GetActorOfClass(GetWorld(), AProtoMech::StaticClass());
	_ProtoMechRef = Cast<AProtoMech>(MechRef);
	
}

void AGrabArm::MoveArm()
{
	// Start the timeline
	if (_GrabbingCurve)
	{
		_ArmTimeline.PlayFromStart();
	}
}

void AGrabArm::UpdateArmPosition(float Alpha)
{
	FVector TargetLocation(1000.0f, 0, 0);
	
	// Calculate the new position
	FVector StartLocation = _ProtoMechRef->_MechMesh->GetComponentLocation();
	FVector EndLocation = StartLocation + TargetLocation;
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
    
	// Set the new position
	SetActorLocation(NewLocation, true, nullptr, ETeleportType::None);
}

void AGrabArm::OnArmTimelineFinished()
{
	
}

// Called every frame
void AGrabArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the timeline
	_ArmTimeline.TickTimeline(DeltaTime);
}

