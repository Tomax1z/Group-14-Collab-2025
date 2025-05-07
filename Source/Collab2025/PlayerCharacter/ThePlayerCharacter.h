// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProtoMech.h"
#include "Kismet/GameplayStatics.h"
#include "Collab2025/PlayerController/Interfaces/IAInterface.h"
#include "ISpeedRatioReceivable.h"
#include "Components/ArrowComponent.h"
#include "ThePlayerCharacter.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class UCharacterMovementComponent;

UCLASS()
class COLLAB2025_API AThePlayerCharacter : public ACharacter, public IIAInterface, public IISpeedRatioReceivable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThePlayerCharacter();

	virtual void IAMove_Implementation(const FInputActionInstance& Instance) override;
	virtual void IALook_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAInteract_Implementation(const FInputActionInstance& Instance) override;
	virtual void IASprint_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStopSprinting_Implementation(const FInputActionInstance& Instance) override;
	virtual void IACrouch_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStand_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> _Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> _Camera;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterMovementComponent> _CharacterMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	TObjectPtr<UArrowComponent> _HoldArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _NumOfOxygenTanks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _NumOfPowerCells;
	

public:
	void SetIsSprinting(bool bSprinting);

	virtual void SetSpeedRatio_Implementation(float SprintSpeed, float BaseSpeed) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float _SprintSpeed = 1000.0f;
	UPROPERTY(EditAnywhere)
	float _WalkSpeed = 500.0f;
	UPROPERTY(EditAnywhere)
	float _CrouchSpeed = 300.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Sprint State")
	bool bIsSprinting = false;
	UPROPERTY()
	bool bIsHoldingObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float _SprintBaseSpeedRatio;

public:
	UFUNCTION(BlueprintCallable, Category="Pickup")
	void PickupObject(AActor* ObjectToPickup);

	UCameraComponent* GetCamera() const
	{
		return _Camera;
	}

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Resources")
	void PlayerReplaceOxygen();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Resources")
	void PlayerReplacePowerCell();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	AProtoMech* GetProtoMech();
	UFUNCTION(BlueprintCallable, Category="Pickup")
	void PlayerSpawnOxygen();
	UFUNCTION(BlueprintCallable, Category="Pickup")
	void PlayerSpawnEnergy();
};