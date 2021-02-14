// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningWIdgetBase.h"
#include <Components/InputComponent.h>
#include <Components/VerticalBox.h>
#include <algorithm>

using namespace std;

bool UOpeningWIdgetBase::Initialize()
{
	if (!InputComponent) {
		InitializeInputComponent();
		if (InputComponent) {
			InputComponent->BindAxis("UIUp", this, &UOpeningWIdgetBase::OnAxisUp);
			InputComponent->BindAction("Talk", IE_Pressed, this, &UOpeningWIdgetBase::OnDecide);
			InputComponent->BindAction("OK", IE_Pressed, this, &UOpeningWIdgetBase::OnDecide);
		}
	}

	currentIndex_ = 0;
	bool ret = Super::Initialize();

	return ret;
}
void UOpeningWIdgetBase::Refocus()
{
	UWidget* w = btnVertBox_->GetChildAt(currentIndex_);
	if (w)
		w->SetFocus();
}
void UOpeningWIdgetBase::OnAxisUp(float val)
{

	if (abs(val) < NeutralThreshold_) {
		hasNeutraled_ = true;
		return;
	}

	if (hasNeutraled_ && abs(val) > MoveThreshold_) {
		if (val > 0)
			--currentIndex_;
		else
			++currentIndex_;

		currentIndex_ = (currentIndex_ + btnVertBox_->GetChildrenCount()) % btnVertBox_->GetChildrenCount();
		hasNeutraled_ = false;
		UE_LOG(LogTemp, Error,TEXT("currentIdx: %d"), currentIndex_);
	}
	return;
}

void UOpeningWIdgetBase::OnDecide()
{
	decideDelegater_.Broadcast(currentIndex_);
}