// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TwoDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ATwoDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATwoDPlayerController();

protected:
	virtual void SetupInputComponent() override;

private:
	void jump();
};
