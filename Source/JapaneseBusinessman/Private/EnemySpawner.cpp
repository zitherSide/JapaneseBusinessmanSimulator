// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Templates/SubclassOf.h"
#include "EnemyBase.h"
#include "Engine/World.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AEnemyBase* AEnemySpawner::Spawn()
{
	if (canSpawn_) {
		FTransform spawnPoint = GetActorTransform() + spawnPoint_;
		spawnPoint.SetScale3D({ 1, 1, 1 });
		AEnemyBase* ret = GetWorld()->SpawnActor<AEnemyBase>(spawnObject_->GetDefaultObject()->GetClass(), spawnPoint);
		ret->SpawnDefaultController();
		canSpawn_ = false;
		return ret;
	}
	return nullptr;
}