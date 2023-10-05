// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums.h"
#include "BaseCharacter.generated.h"

class ABaseEnemy;
class USphereComponent;
class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UMotionWarpingComponent;

UCLASS()
class RPG_PROJ_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class UBaseAnimInstance;
	
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	/*
	 * Components
	 */

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	USphereComponent* CollisionDetectComponent;

	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess=true))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess=true))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=MotionWarping,meta=(AllowPrivateAccess=true))
	UMotionWarpingComponent* MotionWarpingComponent;
	
	/*
	 * Input Mapping
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Input,meta=(AllowPrivateAccess=true))
	UInputAction* VaultAction;
	
	/*
	 * MovementSpeed
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

	/*
	 * Interactive distance
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Interactive,meta=(AllowPrivateAccess=true))
	float VaultOverCheckedDistance;

	/*
	 * Animation Montages
	 */
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Montage,meta=(AllowPrivateAccess=true))
	UAnimMontage* VaultOverMontage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	ABaseEnemy* OverlappedEnemy;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Montage|Assassination")
	TMap<EAssassinationType,UAnimMontage*> DoAssassinationMontageMap;
	
public:
	ABaseCharacter();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	virtual void PlayAssassinationMontage(EAssassinationType AssassinationType);

private:
	void MoveActionTriggered(const FInputActionValue& Value);

	void LookActionTriggered(const FInputActionValue& Value);

	void CrouchButtonPressed(const FInputActionValue& Value);

	void SprintButtonPressed(const FInputActionValue& Value);

	void VaultButtonPressed(const FInputActionValue& Value);
	
	bool FindVaultTarget(FVector& VaultStart,FVector& VaultMiddle,FVector& VaultEnd);

	UFUNCTION()
	void OnMontageEndedHandle(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
