// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "JapaneseBusinessmanGameMode.h"
#include "JapaneseBusinessmanCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AJapaneseBusinessmanGameMode::AJapaneseBusinessmanGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Player/Blueprints/TwoDPlayerBP.TwoDPlayerBP_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AJapaneseBusinessmanGameMode::BeginPlay()
{
	if (widgetClass_) {
		currentWidget_ = CreateWidget<UUserWidget>(GetWorld(), widgetClass_);
		if (currentWidget_) {
			currentWidget_->AddToViewport();
		}
	}
	player_ = Cast<AJapaneseBusinessmanCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	player_->FullRecoverHealth();
	
	Super::BeginPlay();
}

void AJapaneseBusinessmanGameMode::Tick(float deltaSeconds)
{
	if (player_ && player_->isDead()) {
		makePlayerRagdoll();
		gameOverEvent_.Execute();
	}
}

void AJapaneseBusinessmanGameMode::makePlayerRagdoll()
{
	player_->GetCharacterMovement()->DisableMovement();
	player_->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	player_->GetMesh()->SetSimulatePhysics(true);
}

void AJapaneseBusinessmanGameMode::RespawnPlayer()
{
	player_->Destroy(true);
	player_ = GetWorld()->SpawnActor<AJapaneseBusinessmanCharacter>(DefaultPawnClass->GetDefaultObject()->GetClass(), respawnPoint_->GetActorTransform());
	player_->FullRecoverHealth();
	GetWorld()->GetFirstPlayerController()->Possess(player_);
}