// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageBoxBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNextDispatcher);
/**
 * 
 */
UCLASS()
class UMessageBoxBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class UHorizontalBox* horizontalBox_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class UTextBlock* message_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class UImage* image_;
	UPROPERTY(BlueprintAssignable, Category = Controller)
		FNextDispatcher nextDispatcher_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (ExposeOnSpawn = true))
		FText content_;

	bool Initialize() override;

private:
	void OnNext();
	void Debug(float v);
};
