// Fill out your copyright notice in the Description page of Project Settings.

#include "StageSelectWindowBase.h"

#include "SaveData/JapaneseBusinessmanSaveGame.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SizeBox.h>
#include <algorithm>
#include <unordered_map>

using namespace std;
static constexpr float AxisThreshold = 0.6f;
static constexpr float NeutralThreshold = 0.3f;
static const unordered_map<int, FString> StageNames
{
	{1, FString(TEXT("Suburban"))},
	{2, FString(TEXT("Moshpit Train"))},
	{3, FString(TEXT("Ninja Temple"))},
	{4, FString(TEXT("City"))},
};

static const unordered_map<int, FName> LevelNames
{
	{1, FName(TEXT("Staget1"))},
	{2, FName(TEXT("Stage2"))},
	{3, FName(TEXT("Stage3"))},
	{4, FName(TEXT("Stage4"))},
};


static float getAxisValue(float val, bool& neutraled) {
	if (abs(val) < NeutralThreshold) {
		neutraled = true;
		return 0;
	}

	if (neutraled) {
		if (abs(val) > AxisThreshold) {
			neutraled = false;
			return val;
		}
		else {
			return 0;
		}
	}
	return 0;
}

static inline int ringAdd(int a, int b, int ringSize) {
	return (a + b + ringSize) % ringSize;
}

bool UStageSelectWindowBase::Initialize()
{
	saveGame_ = Cast<UJapaneseBusinessmanSaveGame>(UGameplayStatics::LoadGameFromSlot(UJapaneseBusinessmanSaveGame::SlotName, UJapaneseBusinessmanSaveGame::SlotNo));
	if (!saveGame_) {
		saveGame_ = Cast<UJapaneseBusinessmanSaveGame>(UGameplayStatics::CreateSaveGameObject(UJapaneseBusinessmanSaveGame::StaticClass()));
	}

	if (!InputComponent) {
		InitializeInputComponent();
	}
	if (InputComponent) {
		InputComponent->BindAction(TEXT("OK"), IE_Pressed, this, &UStageSelectWindowBase::OnDecide);
		InputComponent->BindAction(TEXT("Cancel"), IE_Pressed, this,  &UStageSelectWindowBase::OnCancel);
		InputComponent->BindAxis(TEXT("UIup"), this, &UStageSelectWindowBase::OnAxisV);
		InputComponent->BindAxis(TEXT("UIright"), this, &UStageSelectWindowBase::OnAxisH);
	}

	return UUserWidget::Initialize();
}

void UStageSelectWindowBase::OnAxisH(float val)
{
	const float value = getAxisValue(val, xNeutraled);
	if (value > 0.1) {
		xPos_ = ringAdd(xPos_, 1, MaxStageNum);
	}
	else if (value < -0.1) {
		xPos_ = ringAdd(xPos_, - 1, MaxStageNum);
	}
}

void UStageSelectWindowBase::OnAxisV(float val)
{
	const float value = getAxisValue(val, yNeutraled);
	if (value > 0.1) {
		yPos_ = ringAdd(yPos_, 1, YSize);
	}
	else if (value < -0.1) {
		yPos_ = ringAdd(yPos_, -1, YSize);
	}
}

void UStageSelectWindowBase::OnDecide()
{
	if (yPos_ == 1)
		cancelDispatcher_.Broadcast();
	else {
		const int stageIndex = xPos_ + 1;
		if (saveGame_ && saveGame_->hasStageCleared(stageIndex))
			decideDispatcher_.Broadcast(stageIndex);
		else
			ngDispatcher_.Broadcast();
	}
}

void UStageSelectWindowBase::OnCancel()
{
	cancelDispatcher_.Broadcast();
}

FText UStageSelectWindowBase::getStageName(int i) const
{
	FString unknown(TEXT("???"));
	if (saveGame_ && saveGame_->hasStageCleared(i)) {
		return FText::FromString(StageNames.at(i));
	}

	return FText::FromString(unknown);
}

void UStageSelectWindowBase::ReFocus()
{
	stageBox1_->SetFocus();
}

int UStageSelectWindowBase::getCursorPos() const
{
	if (yPos_ == 1)
		return -1;
	else
		return xPos_;
}

FName UStageSelectWindowBase::getLevelName() const
{
	const int stageIndex = xPos_ + 1;
	return LevelNames.at(stageIndex);
}