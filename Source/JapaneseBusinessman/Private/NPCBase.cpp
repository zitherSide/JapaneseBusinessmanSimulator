// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include "Animation/AnimMontage.h"
#include <Components/BoxComponent.h>
#include <Blueprint/UserWidget.h>

// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(RootComponent);

	popUpRegion_ = CreateDefaultSubobject<UBoxComponent>(TEXT("popUpRegion"));
	popUpRegion_->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
