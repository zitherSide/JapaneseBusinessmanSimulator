// Fill out your copyright notice in the Description page of Project Settings.

#include "JapaneseBusinessmanSaveGame.h"
#include <algorithm>

using namespace std;

const FString UJapaneseBusinessmanSaveGame::SlotName = TEXT("JBMSaveSlot");

bool UJapaneseBusinessmanSaveGame::hasStageCleared(int stageIndex) 
{
	if (stageCleareds_.Contains(stageIndex)) {
		return stageCleareds_[stageIndex];
	}
	else {
		return false;
	}
}

void UJapaneseBusinessmanSaveGame::setStageCleared(int stageIndex, bool cleared)
{
	if (stageCleareds_.Contains(stageIndex)) {
		stageCleareds_[stageIndex] = cleared;
	}
	else {
		stageCleareds_.Add(stageIndex, cleared);
	}
}

int UJapaneseBusinessmanSaveGame::getStageHighScore(int stageIndex)
{
	if (stageHighScores_.Contains(stageIndex)) {
		return stageHighScores_[stageIndex];
	}
	else {
		return 0;
	}
}

void UJapaneseBusinessmanSaveGame::setStageHighScore(int stageIndex, int score)
{
	if (stageHighScores_.Contains(stageIndex)) {
		stageHighScores_[stageIndex] = max(stageHighScores_[stageIndex], score);
	}
	else {
		stageHighScores_.Add(stageIndex, score);
	}
}