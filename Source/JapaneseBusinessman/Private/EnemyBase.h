// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JapaneseBusinessman/PlayerBase.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDieEvent);

UCLASS()
class AEnemyBase : public APlayerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float hitDamage_ = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		TArray<class ATargetPoint*> destinations_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		bool canDamage_ = true;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = Life)
		FDieEvent dieEvent_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Life)
		float deathRemainingTime_ = 0.5;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
		float getHitDamage() const { return hitDamage_; }

	UFUNCTION(BlueprintCallable)
		void Die(FVector force);

};
