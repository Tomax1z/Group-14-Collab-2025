#include "OxygenTank.h"

AOxygenTank::AOxygenTank()
{
	PrimaryActorTick.bCanEverTick = true;
    
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = _Root;
    
	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Root);
	_Mesh->SetSimulatePhysics(true);
    
	_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Root);
	_Collision->SetCollisionProfileName(TEXT("Grabbable"));
	_Collision->SetGenerateOverlapEvents(true);
}

void AOxygenTank::BeginPlay()
{
	Super::BeginPlay();
}

void AOxygenTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOxygenTank::OnGrabbed_Implementation(AActor* Grabber)
{
	// Disable physics when grabbed
	_Mesh->SetSimulatePhysics(false);
	_CanBeGrabbed = false;
    
	UE_LOG(LogTemp, Warning, TEXT("Oxygen Tank grabbed!"));
}

void AOxygenTank::OnReleased_Implementation(AActor* Grabber)
{
	// Enable physics again
	_Mesh->SetSimulatePhysics(true);
	_CanBeGrabbed = true;
    
	UE_LOG(LogTemp, Warning, TEXT("Oxygen Tank released!"));
}

bool AOxygenTank::CanBeGrabbed_Implementation() const
{
	return _CanBeGrabbed;
}

FString AOxygenTank::GetConsumableType_Implementation() const
{
	return FString("Oxygen");
}
