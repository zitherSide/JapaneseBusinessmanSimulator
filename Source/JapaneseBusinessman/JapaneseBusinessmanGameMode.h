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
	class AJapaneseBusinessmanCharacter* player_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mode)
		FGameOver gameOverEvent_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		class AActor* respawnPoint_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Story)
		TMap<FString, bool> gameFlags_;

	UFUNCTION(BlueprintCallable, Category = Pawn)
		void makePlayerRagdoll();

	UFUNCTION(BlueprintCallable, Category = Pawn)
		void RespawnPlayer();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

};



