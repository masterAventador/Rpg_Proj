// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuinnCharacter.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class RPG_PROJ_API AQuinnCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess= true))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess = true))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess = true))
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess = true))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess = true))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess = true))
	UInputAction* JumpAction;

public:
	AQuinnCharacter();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

};
