// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"
#include "Components/SphereComponent.h"
#include "BaseEnemy.h"

ABaseCharacter::ABaseCharacter():
MaxRunSpeed(500.f),
MaxCrouchSpeed(350.f),
MaxSprintSpeed(700.f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = false;

	CollisionDetectComponent = CreateDefaultSubobject<USphereComponent>("CollisionDetect");
	CollisionDetectComponent->SetupAttachment(GetRootComponent());
	CollisionDetectComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionDetectComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*Montage callback*/
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this,&ThisClass::OnMontageEndedHandle);

	CollisionDetectComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnComponentBeginOverlap);
	CollisionDetectComponent->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnComponentEndOverlap);

	VaultOverCheckedDistance = CollisionDetectComponent->GetScaledSphereRadius();

	MovementComponent = GetCharacterMovement();

	MovementComponent->NavAgentProps.bCanCrouch = true;
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.f,500.f,0.f);
	MovementComponent->MaxWalkSpeed = MaxRunSpeed;
	MovementComponent->MaxWalkSpeedCrouched = MaxCrouchSpeed;
	MovementComponent->SetCrouchedHalfHeight(60.f);
	MovementComponent->MaxAcceleration = 1000.f;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) // bind input mapping
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext,0);
		}
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) // rotate the camera's default angle
	{
		PlayerController->RotationInput -= FRotator(30.f, 0.f, 0.f);
	}

}

void ABaseCharacter::PlayAssassinationMontage(EAssassinationType AssassinationType)
{
	if (!OverlappedEnemy) return;
	
	if (!DoAssassinationMontageMap.Contains(AssassinationType)) return;

	UAnimMontage* AnimMontage = DoAssassinationMontageMap[AssassinationType];
	PlayAnimMontage(AnimMontage);

	OverlappedEnemy->PlayGetAssassinationMontage(AssassinationType);
	
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
		EnhancedInputComponent->BindAction(VaultAction,ETriggerEvent::Started,this,&ThisClass::VaultButtonPressed);
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

void ABaseCharacter::VaultButtonPressed(const FInputActionValue& Value)
{
	PlayAssassinationMontage(EAssassinationType::KickBall);
	return;
	if (!VaultOverMontage) return;
	FVector VaultStart,VaultMiddle,VaultEnd;
	if (FindVaultTarget(VaultStart,VaultMiddle,VaultEnd))
	{
		MovementComponent->MovementMode = MOVE_Flying;
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation("VaultStart",VaultStart);
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation("VaultMiddle",VaultMiddle);
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation("VaultEnd",VaultEnd);
		PlayAnimMontage(VaultOverMontage);
	}
}

bool ABaseCharacter::FindVaultTarget(FVector& VaultStart, FVector& VaultMiddle, FVector& VaultEnd)
{
	const TArray<AActor*> ActorToIngore;
	FHitResult ForwardHitResult;
	bool ForwardHit = false;
	
	for (int i = 0;i < 3;i++)
	{
		FVector ForwardStartLocation = GetActorLocation();
		ForwardStartLocation.Z += i * 30.f;
		FVector ForwardEndLocation = ForwardStartLocation + GetActorForwardVector() * VaultOverCheckedDistance;
		ForwardHit = UKismetSystemLibrary::SphereTraceSingle(this,ForwardStartLocation,ForwardEndLocation,5.f,TraceTypeQuery1,false,ActorToIngore,EDrawDebugTrace::ForDuration,ForwardHitResult,true);
		if (ForwardHit)
		{
			break;
		}
	}

	if (!ForwardHit) return false;
	
	bool StartHit = false, MiddleHit = false,LandHit = false;
	for (int i = 0; i < 5;i++)
	{
		FVector DownwardStartLocation = ForwardHitResult.Location;
		DownwardStartLocation.Z += 100.f;
		DownwardStartLocation += GetActorForwardVector() * i * 80.f;
		FVector DownwardEndLocation = DownwardStartLocation;
		DownwardEndLocation.Z = ForwardHitResult.Location.Z;

		bool Hit = false;
		FHitResult HitResult;
		Hit = UKismetSystemLibrary::SphereTraceSingle(this,DownwardStartLocation,DownwardEndLocation,20.f,TraceTypeQuery1,false,ActorToIngore,EDrawDebugTrace::None,HitResult,true);

		if (Hit)
		{
			if (!StartHit)
			{
				VaultStart = HitResult.ImpactPoint;
				StartHit = true;
			} else
			{
				VaultMiddle = HitResult.ImpactPoint;
				MiddleHit = true;
			}
		} else
		{
			break;
		}
	}

	if (!StartHit || !MiddleHit) return false;

	FHitResult LandHitResult;
	FVector LandStartLocation , LandEndLocation;
	LandStartLocation = VaultMiddle;
	LandStartLocation += GetActorForwardVector() * 230.f;

	LandEndLocation = LandStartLocation;
	LandEndLocation.Z -= 1000.f;
	LandHit = UKismetSystemLibrary::LineTraceSingle(this,LandStartLocation,LandEndLocation,TraceTypeQuery1,false,ActorToIngore,EDrawDebugTrace::None,LandHitResult,true);

	if (LandHit)
	{
		VaultEnd = LandHitResult.Location;
	}

	return LandHit;
}

void ABaseCharacter::OnMontageEndedHandle(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (AnimMontage == VaultOverMontage)
	{
		MovementComponent->MovementMode = MOVE_Walking;
	}	
}

void ABaseCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IInteractionInterface* item = Cast<IInteractionInterface>(OtherActor))
	{
		item->SetInteractionWidgetVisibility(true);
	}
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
	{
		OverlappedEnemy = Enemy;
	}
}

void ABaseCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IInteractionInterface* item = Cast<IInteractionInterface>(OtherActor))
	{
		item->SetInteractionWidgetVisibility(false);
	}
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
	{
		OverlappedEnemy = nullptr;
	}
}

































































