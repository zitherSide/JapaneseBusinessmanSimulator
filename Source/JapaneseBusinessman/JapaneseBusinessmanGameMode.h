// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JapaneseBusinessmanGameMode.generated.h"

DECLARE_DYNAMIC_DELEGATE(FGameOver);

UCLASS(minimalapi)
class AJapaneseBusinessmanGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AJapaneseBusinessmanGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserWidget> widgetClass_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		class UUserWidget* currentWidget_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		class APlayerBase* player_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mode)
		FGameOver gameOverEvent_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		class AActor* respawnPoint_;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = Game)
		int damageCount_;

	UFUNCTION(BlueprintCallable, Category = Pawn)
		void makePlayerRagdoll();

	UFUNCTION(BlueprintCallable, Category = Pawn)
		void RespawnPlayer();

	UFUNCTION(BlueprintCallable, Category = Save)
		void Save();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Story)
		class UJapaneseBusinessmanSaveGame* saveGame_{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
		float timeLimit_{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
		float currentTime_{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
		bool isCountingDown_{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
		bool hasTimeLimit_{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
		class UTimerWidgetBase* timerWidget_;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

};



