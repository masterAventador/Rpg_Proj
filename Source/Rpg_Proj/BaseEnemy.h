// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums.h"
#include "InteractionInterface.h"
#include "BaseEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class RPG_PROJ_API ABaseEnemy : public ACharacter,public IInteractionInterface
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TMap<EAssassinationType,UAnimMontage*> GetAssassinationMontageMap;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	UWidgetComponent* InteractionWidgetComponent;

public:
	ABaseEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetInteractionWidgetVisibility(bool bVisibility) override;

	virtual void SetInteractionWidgetText(FString&& Text) override;

	virtual void GetAssassination(EAssassinationType AssassinationType);

};
