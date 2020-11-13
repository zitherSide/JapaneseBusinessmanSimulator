// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <GameFramework/Actor.h>
#include "ResponseWindowBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecideDelegate, int, responseIndex);
/**
 * 
 */
UCLASS()
class UResponseWindowBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta=(BindWidget))
		class USizeBox* sizeBox_ = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta=(BindWidget))
		class UVerticalBox* responseList_ = nullptr;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = Controller)
		FOnDecideDelegate onDecide_;

	UFUNCTION(BlueprintCallable, Category = Controller)
		void BindController(class APlayerController* controller);

	UFUNCTION(BlueprintCallable, Category = UI)
		void InitFocus();

private:
	class APlayerController* controller_ = nullptr;
	class UInputComponent* inputComp_ = nullptr;
	int currentIndex_ = 0;
	bool detected_ = false;
	static const float Threshold_;

protected:
	virtual void RemoveFromParent() override;

private:
	void OnUpKey(float value);
	void OnDecideKey();
};
