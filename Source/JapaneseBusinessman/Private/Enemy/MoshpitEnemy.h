// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoshpitEnemy.generated.h"

UCLASS()
class AMoshpitEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMoshpitEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float smashThreshold_ = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float smashSpeed_ = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float nudgeSpeed_ = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Power)
		float hitDamage_ = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movemnt)
		bool canDamage_ = true;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
