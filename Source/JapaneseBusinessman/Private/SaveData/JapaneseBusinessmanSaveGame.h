// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "JapaneseBusinessmanSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UJapaneseBusinessmanSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Stage)
		TMap<int, bool> stageCleareds_;
	UPROPERTY(VisibleAnywhere, Category = Stage)
		TMap<int, int> stageHighScores_;

	static const FString SlotName;
	static const int32 SlotNo = 0;

	UJapaneseBusinessmanSaveGame() {}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Stage)
		bool hasStageCleared(int stageIndex);
	UFUNCTION(BlueprintCallable, Category = Stage)
		void setStageCleared(int stageIndex, bool cleared);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Stage)
		int getStageHighScore(int stageIndex);
	UFUNCTION(BlueprintCallable, Category = Stage)
		void setStageHighScore(int stageIndex, int score);
};
