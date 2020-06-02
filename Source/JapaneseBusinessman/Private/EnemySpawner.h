// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
		TSubclassOf<class AEnemyBase> spawnObject_;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	//	TSubclassOf<class AIController> spawnController_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn, meta = (MakeEditWidget = true))
		FTransform spawnPoint_;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = Spawn)
		AEnemyBase* Spawn();
	UFUNCTION(BlueprintCallable, Category = Spawn)
		void ResetCanSpawn() { canSpawn_ = true; }

private:
	bool canSpawn_ = true;
};
