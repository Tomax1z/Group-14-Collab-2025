#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

// Forward declare the interface UObject class
class USpeedControllable;

UCLASS()
class COLLAB2025_API ALever : public AActor, public IIInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> _RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> _LeverMeshBase;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> _LeverMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lever Settings")
	TObjectPtr<AActor> _TargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lever State")
	bool _bIsPulled = false; // State of the lever (Pulled = Sprinting, Not Pulled = Base Speed)

	UPROPERTY(EditAnywhere, Category = "Lever|Rotation")
	FRotator PulledRotation = FRotator(0.f, 0.f, 45.f);

	UPROPERTY(EditAnywhere, Category = "Lever|Rotation")
	FRotator DefaultRotation = FRotator::ZeroRotator;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to simulate pulling/pushing the lever
	UFUNCTION(BlueprintCallable, Category = "Lever Actions")
	void ToggleLever();

	virtual void Interact_Implementation(AActor* Interactor) override;

private:
	// Helper function to notify the target actor
	void NotifyTargetActor();
};
