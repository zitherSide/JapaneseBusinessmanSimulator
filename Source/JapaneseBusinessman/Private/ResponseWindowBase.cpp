// Fill out your copyright notice in the Description page of Project Settings.

#include "ResponseWindowBase.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/Engine.h>
#include <GameFramework/PlayerController.h>
#include <GameFramework/PlayerInput.h>
#include <Components/InputComponent.h>
#include <Components/VerticalBox.h>
#include <string>
#include "TextButtonBase.h"
#include <thread>

using namespace std;
const float UResponseWindowBase::Threshold_ = 0.8f;

void UResponseWindowBase::BindController(APlayerController* controller)
{
	//InitializeでUGameStaticsからはコントローラーが取れなかった...なぜ？
	UE_LOG(LogTemp, Log, TEXT("InitResponseWindow!"));

	inputComp_ = NewObject<UInputComponent>();
	if (inputComp_) {
		inputComp_->BindAxis("UIup", this, &UResponseWindowBase::OnUpKey);
		inputComp_->BindAction("Talk", IE_Pressed, this, &UResponseWindowBase::OnDecideKey);
	}
	else
		UE_LOG(LogTemp, Log, TEXT("Could not new input component"));

	controller_ = controller;
	controller_->PushInputComponent(inputComp_);

	return;
}

void UResponseWindowBase::RemoveFromParent()
{
	if(inputComp_ && controller_)
		controller_->PopInputComponent(inputComp_);
	Super::RemoveFromParent();
}

void UResponseWindowBase::InitFocus()
{
	responseList_->GetChildAt(currentIndex_)->SetFocus();
}

void UResponseWindowBase::OnUpKey(float value)
{
	if (!detected_ && value > Threshold_) {
		detected_ = true;
		currentIndex_ = (currentIndex_ + 1) % responseList_->GetChildrenCount();
		responseList_->GetChildAt(currentIndex_)->SetFocus();
	}
	else if (!detected_ && value < -Threshold_) {
		detected_ = true;
		currentIndex_ = (currentIndex_ - 1 + responseList_->GetChildrenCount()) % responseList_->GetChildrenCount();
		responseList_->GetChildAt(currentIndex_)->SetFocus();
	}
	else if (abs(value) < 0.2) {
		detected_ = false;
	}
}

void UResponseWindowBase::OnDecideKey()
{
	onDecide_.Broadcast(currentIndex_);
}
