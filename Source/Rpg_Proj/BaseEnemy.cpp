// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "BaseCharacter.h"
#include "InteractionUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextBlock.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Interaction");
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());
	InteractionWidgetComponent->SetVisibility(false);

	AssassinationDetectBox = CreateDefaultSubobject<UBoxComponent>("AssassinationDetection");
	AssassinationDetectBox->SetupAttachment(GetRootComponent());

}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetInteractionWidgetText("Press F to Kick his balls");

	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this,&ThisClass::OnMontageEndedHandle);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	AssassinationDetectBox->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::AssassinationBoxOnBeginOverlapped);
	AssassinationDetectBox->OnComponentEndOverlap.AddDynamic(this,&ThisClass::AssassinationBoxOnEndOverlaypped);
}

void ABaseEnemy::ChangCollisionEnabled(ECollisionEnabled::Type NewType)
{
	GetCapsuleComponent()->SetCollisionEnabled(NewType);
}

void ABaseEnemy::AssassinationBoxOnBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		FVector Location = GetActorLocation();
		Location -= GetActorForwardVector() * 120.f;
		
		Character->AssassinationEnemy = this;
		Character->AssassinationLocation = Location;
		SetInteractionWidgetVisibility(true);
		DrawDebugSphere(GetWorld(),Location,5.f,3,FColor::Emerald,true);
	}
}

void ABaseEnemy::AssassinationBoxOnEndOverlaypped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		Character->AssassinationEnemy = nullptr;
		SetInteractionWidgetVisibility(false);
	}
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
		ChangCollisionEnabled(ECollisionEnabled::NoCollision);
		UAnimMontage* Montage = GetAssassinationMontageMap[AssassinationType];
		PlayAnimMontage(Montage);
	}
}

void ABaseEnemy::OnMontageEndedHandle(UAnimMontage* Montage, bool bInterrupted)
{
	ChangCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
