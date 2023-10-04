// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "InteractionUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Interaction");
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());
	InteractionWidgetComponent->SetVisibility(false);
}

void ABaseItem::SetInteractionWidgetVisibility(bool bVisibility)
{
	InteractionWidgetComponent->SetVisibility(bVisibility);
}

void ABaseItem::SetInteractionWidgetText(FString&& Text)
{
	if (UInteractionUserWidget* InteractionUserWidget = Cast<UInteractionUserWidget>(InteractionWidgetComponent->GetWidget()))
	{
		InteractionUserWidget->InteractionTextBlock->SetText(FText::FromString(Text));
	}
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	SetInteractionWidgetText("Press F to VaultOver");
}

