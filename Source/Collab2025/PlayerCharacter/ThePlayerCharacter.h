// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Collab2025/PlayerController/Interfaces/IAInterface.h"
#include "ThePlayerCharacter.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class UCharacterMovementComponent;

UCLASS()
class COLLAB2025_API AThePlayerCharacter : public ACharacter, public IIAInterface
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float _SprintSpeed = 1000.0f;
	UPROPERTY(EditAnywhere)
	float _WalkSpeed = 500.0f;
	UPROPERTY(EditAnywhere)
	float _CrouchSpeed = 300.0f;

public:

	UCameraComponent* GetCamera() const
	{
		return _Camera;
	}
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};