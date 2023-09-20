// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class RPG_PROJ_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class UBaseAnimInstance;
	
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	/*
	 * Camera
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess=true))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess=true))
	UCameraComponent* FollowCamera;
	
	/*
	 * Input Mapping
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* SprintAction;
	
	/*
	 * Movement
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement,meta=(AllowPrivateAccess=true))
	float MaxRunSpeed;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement,meta=(AllowPrivateAccess=true))
	float MaxCrouchSpeed;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement,meta=(AllowPrivateAccess=true))
	float MaxSprintSpeed;

	/*
	 * Character State
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement,meta=(AllowPrivateAccess=true))
	bool bIsSprinting;

public:
	ABaseCharacter();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MoveActionTriggered(const FInputActionValue& Value);

	void LookActionTriggered(const FInputActionValue& Value);

	void CrouchButtonPressed(const FInputActionValue& Value);

	void SprintButtonPressed(const FInputActionValue& Value);

};
