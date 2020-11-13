// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpeningWIdgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDecideDelegater, int, index);
/**
 * 
 */
UCLASS()
class UOpeningWIdgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	bool Initialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = UI)
		class UVerticalBox* btnVertBox_;
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = Controller)
		FDecideDelegater decideDelegater_;
	UFUNCTION(BlueprintCallable, Category = UI)
		void Refocus();
private:
	class APlayerController* controller_ = nullptr;
	class UInputComponent* input_ = nullptr;
	int currentIndex_ = 0;
	
	void OnAxisUp(float val);
	void OnDecide();

	bool hasNeutraled_ = true;
	const float NeutralThreshold_ = 0.2f;
	const float MoveThreshold_ = 0.6f;
};
