// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "InteractionUserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Interaction");
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());
	InteractionWidgetComponent->SetVisibility(false);
	
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetInteractionWidgetText("Press F to Kick his balls");
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEnemy::SetInteractionWidgetVisibility(bool bVisibility)
{
	InteractionWidgetComponent->SetVisibility(bVisibility);	
}

void ABaseEnemy::SetInteractionWidgetText(FString&& Text)
{
	if (UInteractionUserWidget* InteractionUserWidget = Cast<UInteractionUserWidget>(InteractionWidgetComponent->GetWidget()))
	{
		InteractionUserWidget->InteractionTextBlock->SetText(FText::FromString(Text));
	}
}

void ABaseEnemy::GetAssassination(EAssassinationType AssassinationType)
{
	if (GetAssassinationMontageMap.Contains(AssassinationType))
	{
		UAnimMontage* Montage = GetAssassinationMontageMap[AssassinationType];
		PlayAnimMontage(Montage);
	}
}
