// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabArm.h"

#include "IGrabbable.h"
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
}

// Called when the game starts or when spawned
void AGrabArm::BeginPlay()
{
	Super::BeginPlay();

	_ArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrabArm::OnArmCollisionBeginOverlap);
	_IsHoldingObject = false;
	_GrabbedObject = nullptr;

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
	FVector TargetLocation(GetActorForwardVector() * 1000);
	
	// Calculate the new position
	FVector StartLocation = _ProtoMechRef->_MechMesh->GetComponentLocation();
	FVector EndLocation = StartLocation + TargetLocation;
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
    
	// Set the new position
	SetActorLocation(NewLocation, true, nullptr, ETeleportType::None);
}

void AGrabArm::OnArmTimelineFinished()
{
	// If the arm has returned to the mech and is holding an object, release it
	if (_IsHoldingObject && _ArmTimeline.GetTimelineLength() - _ArmTimeline.GetPlaybackPosition() < 0.1f)
	{
		ReleaseObject();
	}
}

// Called every frame
void AGrabArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the timeline
	_ArmTimeline.TickTimeline(DeltaTime);
}

void AGrabArm::OnArmCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!_IsHoldingObject && OtherActor)
    {
    	if (OtherActor->GetClass()->ImplementsInterface(UIGrabbable::StaticClass()))
    	{
    		// Use Execute_ method to call the interface function
    		bool CanGrab = IIGrabbable::Execute_CanBeGrabbed(OtherActor);
    
    		if (CanGrab)
    		{
    			GrabObject(OtherActor);
    			_ArmTimeline.Reverse();
    		}
    	}

    	/*
        // Try to cast to the IGrabbable interface
        IIGrabbable* GrabbableObject = Cast<IIGrabbable>(OtherActor);
        if (GrabbableObject && GrabbableObject->CanBeGrabbed())
        {
            GrabObject(OtherActor);
            
            // Start retracting arm to bring object back
            _ArmTimeline.Reverse();
        }
        */
    }
}

void AGrabArm::GrabObject(AActor* ObjectToGrab)
{
	if (ObjectToGrab)
	{
		// Store reference to grabbed object
		_GrabbedObject = ObjectToGrab;
		_IsHoldingObject = true;
        
		// Notify the object it's been grabbed - FIXED VERSION
		if (ObjectToGrab->GetClass()->ImplementsInterface(UIGrabbable::StaticClass()))
		{
			IIGrabbable::Execute_OnGrabbed(ObjectToGrab, this);
		}
        
		// Attach object to arm
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, false);
		ObjectToGrab->AttachToComponent(_ArmMesh, AttachRules);
        
		UE_LOG(LogTemp, Warning, TEXT("Object grabbed: %s"), *ObjectToGrab->GetName());
	}
}


void AGrabArm::ReleaseObject()
{
	if (_GrabbedObject)
	{
		// Notify the object it's being released (FIXED VERSION)
		if (_GrabbedObject->GetClass()->ImplementsInterface(UIGrabbable::StaticClass()))
		{
			// Use Execute_ method instead of direct call
			IIGrabbable::Execute_OnReleased(_GrabbedObject, this);
            
			// Be careful: you might also need to fix this line if GetConsumableType
			// is a BlueprintImplementableEvent or BlueprintNativeEvent
			FString ConsumableType = IIGrabbable::Execute_GetConsumableType(_GrabbedObject);
			UE_LOG(LogTemp, Warning, TEXT("Consuming %s"), *ConsumableType);
            
			// Add implementation to handle consumption based on type
			// You can inform the mech here about the consumed resource
		}
        
		// Detach object
		_GrabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        
		// Reset grabbed object tracking
		_GrabbedObject = nullptr;
		_IsHoldingObject = false;
	}
}

