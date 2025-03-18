#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IGrabbable.h"
#include "Components/BoxComponent.h"
#include "EnergyCell.generated.h"

UCLASS()
class COLLAB2025_API AEnergyCell : public AActor, public IIGrabbable
{
	GENERATED_BODY()
    
public:
	AEnergyCell();
    
protected:
	virtual void BeginPlay() override;
    
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> _Root;
    
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> _Mesh;
    
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> _Collision;
    
	UPROPERTY(EditAnywhere, Category = "Consumable")
	float _EnergyAmount = 100.0f;
    
	bool _CanBeGrabbed = true;
    
public:
	virtual void Tick(float DeltaTime) override;
    
	// IGrabbable Interface Implementation
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	void OnGrabbed(AActor* Grabber);
	virtual void OnGrabbed_Implementation(AActor* Grabber) override;
    
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	void OnReleased(AActor* Grabber);
	virtual void OnReleased_Implementation(AActor* Grabber) override;
    
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	bool CanBeGrabbed() const;
	virtual bool CanBeGrabbed_Implementation() const override;
    
	UFUNCTION(BlueprintNativeEvent, Category = "Grabbable")
	FString GetConsumableType() const;
	virtual FString GetConsumableType_Implementation() const override;
};
