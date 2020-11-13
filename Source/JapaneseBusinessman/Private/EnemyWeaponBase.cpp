// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponBase.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
AEnemyWeaponBase::AEnemyWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileMovement_ = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	arrow_ = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow_->SetupAttachment(RootComponent);

	if (RootComponent) {
		RootComponent->ComponentTags.Add("EnemyWeapon");
	}
}

// Called when the game starts or when spawned
void AEnemyWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyWeaponBase::SetInitialSpeed(float speed)
{
	if (projectileMovement_) {
		projectileMovement_->InitialSpeed = speed;
	}
}