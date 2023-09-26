// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"

#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character) return;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	
	GroundSpeed = MovementComponent->Velocity.Size2D();
	
	ZSpeed = MovementComponent->Velocity.Z;
	
	bIsMoving = GroundSpeed > 0;

	bIsJumping = MovementComponent->IsFalling();

	bIsCrouching = Character->bIsCrouched;

	bIsSprinting = Character->bIsSprinting;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,-1,FColor::Red,FString::Printf(TEXT("current speed:%f"),GroundSpeed));
	}

}

void UBaseAnimInstance::playAnimationMontage(UAnimMontage* Montage)
{
	if (!Montage) return;
	Montage_Play(Montage);
}
