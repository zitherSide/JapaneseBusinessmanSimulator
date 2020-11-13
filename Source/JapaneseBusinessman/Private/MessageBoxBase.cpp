// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageBoxBase.h"

void UMessageBoxBase::BindController(APlayerController * controller)
{
	controller_ = controller;

	input_ = NewObject<UInputComponent>();
	input_->BindAction("Talk", IE_Pressed, this, &UMessageBoxBase::OnNext);

	controller_->PushInputComponent(input_);
}

void UMessageBoxBase::RemoveFromParent()
{
	if(controller_ && input_)
		controller_->PopInputComponent(input_);
	Super::RemoveFromParent();
}

void UMessageBoxBase::OnNext()
{
	nextDispatcher_.Broadcast();
}

void UMessageBoxBase::Debug(float v)
{
	UE_LOG(LogTemp, Log, TEXT("input %f"), v);
}