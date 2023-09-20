// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter():
MaxRunSpeed(500.f),
MaxCrouchSpeed(350.f),
MaxSprintSpeed(700.f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetCharacterMovement();

	MovementComponent->NavAgentProps.bCanCrouch = true;
	
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.f,500.f,0.f);
	MovementComponent->MaxWalkSpeed = MaxRunSpeed;
	MovementComponent->MaxWalkSpeedCrouched = MaxCrouchSpeed;
	MovementComponent->SetCrouchedHalfHeight(60.f);
	MovementComponent->MaxAcceleration = 1000.f;

	MovementComponent->NavAgentProps.bCanCrouch = true;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext,0);
		}
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->RotationInput -= FRotator(30.f, 0.f, 0.f);
	}
}



void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveActionTriggered);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookActionTriggered);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(CrouchAction,ETriggerEvent::Started,this,&ThisClass::CrouchButtonPressed);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Started,this,&ThisClass::SprintButtonPressed);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Completed,this,&ThisClass::SprintButtonPressed);
	}

}

void ABaseCharacter::MoveActionTriggered(const FInputActionValue& Value)
{
	
	if (Controller)
	{
		FVector2D MovementVector = Value.Get<FVector2D>();
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection,MovementVector.X);
		AddMovementInput(RightDirection,MovementVector.Y);
	}
} 

void ABaseCharacter::LookActionTriggered(const FInputActionValue& Value)
{
	if (Controller)
	{
		FVector2D LookVector = Value.Get<FVector2D>();
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ABaseCharacter::CrouchButtonPressed(const FInputActionValue& Value)
{
	if (bIsCrouched)
	{
		UnCrouch();
	} else
	{
		Crouch();
	}
}

void ABaseCharacter::SprintButtonPressed(const FInputActionValue& Value)
{
	MovementComponent->MaxWalkSpeed = bIsSprinting ? MaxRunSpeed : MaxSprintSpeed;
	bIsSprinting = !bIsSprinting;
}



































































