// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerHealthHUDBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		class AJapaneseBusinessmanCharacter* playerPawn_;
};
