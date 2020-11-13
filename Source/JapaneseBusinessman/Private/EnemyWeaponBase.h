// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWeaponBase.generated.h"

UCLASS()
class AEnemyWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyWeaponBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float damage_ = 10;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* projectileMovement_;

	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* arrow_;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Movement)
		void SetInitialSpeed(float speed);

};
