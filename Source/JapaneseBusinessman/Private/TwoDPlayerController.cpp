// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoDPlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

ATwoDPlayerController::ATwoDPlayerController()
{

}

void ATwoDPlayerController::SetupInputComponent()
{
	if (InputComponent) {
		InputComponent->BindAction("Jump", IE_Pressed, this, &ATwoDPlayerController::jump);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Null"));
		InputComponent = NewObject<UInputComponent>(this, TEXT("2DPayerInput"));
		InputComponent->RegisterComponent();
	}
}

void ATwoDPlayerController::jump()
{
	UE_LOG(LogTemp, Log, TEXT("Jump"));
}