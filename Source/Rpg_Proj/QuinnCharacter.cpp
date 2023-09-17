// Fill out your copyright notice in the Description page of Project Settings.


#include "QuinnCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AQuinnCharacter::AQuinnCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 500.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);

}

void AQuinnCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AQuinnCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuinnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

