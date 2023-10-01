// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_PROJ_API UInteractionUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* InteractionTextBlock;
};
