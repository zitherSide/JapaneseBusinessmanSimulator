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

bool UResponseWindowBase::Initialize()
{
	if (!InputComponent) {
		InitializeInputComponent();
		if (InputComponent) {
			InputComponent->BindAxis("UIup", this, &UResponseWindowBase::OnUpKey);
			InputComponent->BindAction("Talk", IE_Pressed, this, &UResponseWindowBase::OnDecideKey);
		}
	}

	return Super::Initialize();
}

void UResponseWindowBase::Refocus()
{
	if (responseList_) {
		auto* elem = responseList_->GetChildAt(currentIndex_);
		if(elem)
			elem->SetFocus();
	}
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
