// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/EngineTypes.h>
#include "MovingActorBase.generated.h"

UCLASS()
class AMovingActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingActorBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float speed_{ 300.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float waitTime_{ 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float offsetTime_{ 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		TArray<class ATargetPoint*> destinations_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* mesh_;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		class UBoxComponent* box_;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float destMargin_{ 20.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float crushMargin_{ 100.f };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int nextDestination_ = 0;
	bool isWaiting_{ false };

	FTimerHandle waitTimer_;
	void clearIsWaiting() { isWaiting_ = false; }
};
