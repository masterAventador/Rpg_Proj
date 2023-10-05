// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums.h"
#include "InteractionInterface.h"
#include "BaseEnemy.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class RPG_PROJ_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TMap<EAssassinationType,UAnimMontage*> GetAssassinationMontageMap;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	UWidgetComponent* InteractionWidgetComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	UBoxComponent* AssassinationDetectBox;

public:
	ABaseEnemy();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetInteractionWidgetVisibility(bool bVisibility);

	virtual void SetInteractionWidgetText(FString&& Text);

	virtual void GetAssassination(EAssassinationType AssassinationType);

	UFUNCTION()
	void OnMontageEndedHandle(UAnimMontage* Montage, bool bInterrupted);

	
protected:
	virtual void BeginPlay() override;
private:
	void ChangCollisionEnabled(ECollisionEnabled::Type NewType);

	UFUNCTION()
	void AssassinationBoxOnBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void AssassinationBoxOnEndOverlaypped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
