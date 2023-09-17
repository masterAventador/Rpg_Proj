// Fill out your copyright notice in the Description page of Project Settings.


#include "QuinnCharacter.h"

// Sets default values
AQuinnCharacter::AQuinnCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQuinnCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuinnCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AQuinnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

