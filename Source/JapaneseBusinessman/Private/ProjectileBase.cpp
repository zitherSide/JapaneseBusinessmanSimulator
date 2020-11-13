// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	root_ = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root_;

	arrow_ = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow_->SetupAttachment(RootComponent);

	projectile_ = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	//mesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh_->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(lifespan_);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

