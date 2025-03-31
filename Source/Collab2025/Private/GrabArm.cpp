// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabArm.h"

#include "IConsumable.h"
#include "ProtoMech.h"
#include "Collab2025/PlayerCharacter/ThePlayerCharacter.h"
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

	_ArmCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	_ArmCollision->SetGenerateOverlapEvents(true);
	_ArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrabArm::OnArmOverlapBegin);
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
	PlayerCharacter = Cast<AThePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	AActor* MechRef = UGameplayStatics::GetActorOfClass(GetWorld(), AProtoMech::StaticClass());
	_ProtoMechRef = Cast<AProtoMech>(MechRef);
	
}

void AGrabArm::MoveArm()
{
	// Clear any previous grab target
	if (!_bIsGrabbing)
	{
		_GrabbedObject = nullptr;
	}
    
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

	// Try to grab
	if (!_bIsGrabbing && _GrabbedObject)
	{
		TryGrab();
	}
}

void AGrabArm::OnArmTimelineFinished()
{
	// If we didn't grab anything during extension, clear any potential grab target
	if (!_bIsGrabbing)
	{
		_GrabbedObject = nullptr;
	}

	if (_bIsGrabbing)
	{
		ReleaseGrabbedObject();
	}
}

// Called every frame
void AGrabArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
	// Update the timeline
	_ArmTimeline.TickTimeline(DeltaTime);
}

void AGrabArm::OnArmOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If we're already grabbing something or the overlap is invalid, ignore this overlap
	if (_bIsGrabbing || !OtherActor)
	{
		UE_LOG(LogTemp, Display, TEXT("GrabFull"));
		return;
	}

	// Check if the actor implements the IGrabbable interface
	if (OtherActor->GetClass()->ImplementsInterface(UIGrabbable::StaticClass()))
	{
		// Check if it can be grabbed (optional)
		//bool bCanBeGrabbed = IIGrabbable::Execute_CanBeGrabbed(OtherActor);

		//UE_LOG(LogTemp, Log, TEXT("%s CanBeGrabbed %s"), *OtherActor->GetName(), bCanBeGrabbed ? TEXT("TRUE") : TEXT("FALSE"));
		_GrabbedObject = OtherActor;
		UE_LOG(LogTemp, Display, TEXT("GrabbedInterface"));

		/*
		if (bCanBeGrabbed)
		{
			UE_LOG(LogTemp, Log, TEXT("Setting %s as potential grab target"), *OtherActor->GetName());
			
			// We found a grabbable object that's overlapping
			_GrabbedObject = OtherActor;
			
		}
		*/
	}
}

bool AGrabArm::TryGrab()
{
	if (_GrabbedObject)
	{
		// We have a valid object to grab
		_bIsGrabbing = true;

		// Notify the object it's been grabbed
		IIGrabbable::Execute_OnGrabbed(_GrabbedObject, this);

		// Attach the object to the arm
		UStaticMeshComponent* GrabbableMeshComponent = Cast<UStaticMeshComponent>(_GrabbedObject->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (GrabbableMeshComponent)
		{
			UE_LOG(LogTemp, Display, TEXT("InRoot"));
			
			// If it has physics, we need to handle it differently
			if (GrabbableMeshComponent->IsSimulatingPhysics())
			{
				// Disable physics and attach
				GrabbableMeshComponent->SetSimulatePhysics(false);
			}
            
			// Attach to arm
			FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, false);
			_GrabbedObject->AttachToComponent(_ArmMesh, AttachRules);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Not in root"));
		}

		return true;
	}
	

	return false;
}

void AGrabArm::ReleaseGrabbedObject()
{
	if (_GrabbedObject)
	{
		// Notify the object it's been released
		IIGrabbable::Execute_OnReleasedGrab(_GrabbedObject, this);
		
		// Detach from the arm
		_GrabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        
		// Re-enable physics if needed
		UPrimitiveComponent* GrabbableRootComponent = Cast<UPrimitiveComponent>(_GrabbedObject->GetRootComponent());
		if (GrabbableRootComponent)
		{
			GrabbableRootComponent->SetSimulatePhysics(true);
		}
		
		if(_GrabbedObject->GetClass()->ImplementsInterface(UIConsumable::StaticClass()))
		{
			FName ConsumableType = IIConsumable::Execute_GetConsumableType(_GrabbedObject);
			UE_LOG(LogTemp, Warning, TEXT("Consumable Type: %s"), *ConsumableType.ToString());

			if (PlayerCharacter && ConsumableType == "OxygenTank")
			{
				PlayerCharacter->_NumOfOxygenTanks += 1.0f;
				_GrabbedObject->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("Oxygen Tanks Now: %f"), PlayerCharacter->_NumOfOxygenTanks);
			}
			if (PlayerCharacter && ConsumableType == "EnergyCell")
			{
				PlayerCharacter->_NumOfOxygenTanks += 1.0f;
				_GrabbedObject->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("Energy Cells Now: %f"), PlayerCharacter->_NumOfOxygenTanks);
			}
		}
		
		_GrabbedObject = nullptr;
		_bIsGrabbing = false;
	}
}
