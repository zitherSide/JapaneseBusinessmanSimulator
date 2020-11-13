// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextButtonBase.generated.h"

/**
 * 
 */
UCLASS()
class UTextButtonBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class USizeBox* SizeBox_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class UButton* responseButton_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class UTextBlock* ResponseText_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (ExposeOnSpawn = true))
		FText content_;
};
