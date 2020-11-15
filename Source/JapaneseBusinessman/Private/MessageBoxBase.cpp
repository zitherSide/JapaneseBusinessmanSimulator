// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageBoxBase.h"

bool UMessageBoxBase::Initialize()
{
	if (!InputComponent) {
		InitializeInputComponent();
		if (InputComponent) {
			InputComponent->BindAction("Talk", IE_Pressed, this, &UMessageBoxBase::OnNext);
		}
	}

	return Super::Initialize();
}

void UMessageBoxBase::OnNext()
{
	nextDispatcher_.Broadcast();
}

void UMessageBoxBase::Debug(float v)
{
	UE_LOG(LogTemp, Log, TEXT("input %f"), v);
}