// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <algorithm>
#include "PlayerBase.generated.h"

UCLASS()
class JAPANESEBUSINESSMAN_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Status)
		int maxHealth_ = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int currentHealth_;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void FullRecoverHealth() { currentHealth_ = maxHealth_; }

	UFUNCTION(BlueprintCallable)
		void addHealth(int health) { currentHealth_ = std::min(maxHealth_, currentHealth_ + health); }

	UFUNCTION(BlueprintPure)
		bool isDead() const { return currentHealth_ <= 0; }


};