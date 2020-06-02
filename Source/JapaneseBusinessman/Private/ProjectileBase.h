// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS(BlueprintType)
class AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USkeletalMeshComponent* mesh_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		class UCapsuleComponent* hitVolume_;

	class USceneComponent* root_;
	class UArrowComponent* arrow_;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		class UProjectileMovementComponent* projectile_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float damage_ = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float lifespan_ = 1.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
