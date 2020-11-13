// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class AInGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = CharacterState)
		int health_;
	UPROPERTY(EditAnywhere, Category = CharacterState)
		int maxHealth_;

	UFUNCTION(BlueprintCallable, Category = CharacterState)
		void setMaxHealth(int maxHealth) { maxHealth_ = maxHealth; }
	
	UFUNCTION(BlueprintCallable, Category = CharacterState)
		void setCurrentHealth(int health) { health_ = health; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		class AJapaneseBusinessmanCharacter* playerPawn_;
};
