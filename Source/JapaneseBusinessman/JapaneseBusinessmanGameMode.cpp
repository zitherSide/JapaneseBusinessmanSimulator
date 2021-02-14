// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "JapaneseBusinessmanGameMode.h"
#include "JapaneseBusinessmanCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SaveData/JapaneseBusinessmanSaveGame.h"
#include "Widgets/TimerWidgetBase.h"

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
	saveGame_ = Cast<UJapaneseBusinessmanSaveGame>(UGameplayStatics::LoadGameFromSlot(UJapaneseBusinessmanSaveGame::SlotName, UJapaneseBusinessmanSaveGame::SlotNo));
	if (!saveGame_) {
		saveGame_ = Cast<UJapaneseBusinessmanSaveGame>(UGameplayStatics::CreateSaveGameObject(UJapaneseBusinessmanSaveGame::StaticClass()));
		UE_LOG(LogTemp, Warning, TEXT("Couldn't load saveGame.(%x)"), saveGame_);
	}

	if (widgetClass_) {
		currentWidget_ = CreateWidget<UUserWidget>(GetWorld(), widgetClass_);
		if (currentWidget_) {
			currentWidget_->AddToViewport();
		}
	}
	player_ = Cast<APlayerBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	player_->FullRecoverHealth();
	
	Super::BeginPlay();
}

void AJapaneseBusinessmanGameMode::Tick(float deltaSeconds)
{
	if (player_ && player_->isDead()) {
		makePlayerRagdoll();
		gameOverEvent_.Execute();
	}

	if (isCountingDown_) {
		currentTime_ -= deltaSeconds;
		if (timerWidget_) {
			timerWidget_->time_ = currentTime_;
		}

		if (currentTime_ <= 0) {
			currentTime_ = 0;
			if(timerWidget_) timerWidget_->time_ = 0;
			isCountingDown_ = false;
			gameOverEvent_.ExecuteIfBound();
		}
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
	player_ = GetWorld()->SpawnActor<APlayerBase>(DefaultPawnClass->GetDefaultObject()->GetClass(), respawnPoint_->GetActorTransform());
	player_->FullRecoverHealth();
	GetWorld()->GetFirstPlayerController()->Possess(player_);
}

void AJapaneseBusinessmanGameMode::Save()
{
	if (saveGame_) {
		const bool res = UGameplayStatics::SaveGameToSlot(saveGame_, UJapaneseBusinessmanSaveGame::SlotName, UJapaneseBusinessmanSaveGame::SlotNo);
		if (res) {
			UE_LOG(LogTemp, Log, TEXT("Game saved."));
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Coudn't save."));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Coudn't save, saveGame_ is null."));
	}
}