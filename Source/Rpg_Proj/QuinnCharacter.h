// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuinnCharacter.generated.h"

UCLASS()
class RPG_PROJ_API AQuinnCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AQuinnCharacter();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera;
	

public:	
	

};
