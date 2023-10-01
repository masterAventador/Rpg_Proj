// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class RPG_PROJ_API ABaseItem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	UWidgetComponent* InteractionWidgetComponent;
	
public:	
	ABaseItem();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetWidgetText(FString&& textString);

};
