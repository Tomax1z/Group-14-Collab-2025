#include "EnergyCell.h"

AEnergyCell::AEnergyCell()
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

void AEnergyCell::BeginPlay()
{
	Super::BeginPlay();
}

void AEnergyCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnergyCell::OnGrabbed_Implementation(AActor* Grabber)
{
	// Disable physics when grabbed
	_Mesh->SetSimulatePhysics(false);
	_CanBeGrabbed = false;
    
	UE_LOG(LogTemp, Warning, TEXT("Energy Cell grabbed!"));
}

void AEnergyCell::OnReleased_Implementation(AActor* Grabber)
{
	// Enable physics again
	_Mesh->SetSimulatePhysics(true);
	_CanBeGrabbed = true;
    
	UE_LOG(LogTemp, Warning, TEXT("Energy Cell released!"));
}

bool AEnergyCell::CanBeGrabbed_Implementation() const
{
	return _CanBeGrabbed;
}

FString AEnergyCell::GetConsumableType_Implementation() const
{
	return FString("Energy");
}
