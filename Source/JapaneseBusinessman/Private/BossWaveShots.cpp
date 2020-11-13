// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWaveShots.h"
#include "BossBase.h"
#include "EnemyWeaponBase.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <AIModule/Classes/AIController.h>
#include <Components/ArrowComponent.h>
#include <random>

EBTNodeResult::Type UBossWaveShots::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	numWaves_ = 0;

	bNotifyTick = true;
	ABossBase* boss = Cast<ABossBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!boss)
		return EBTNodeResult::Failed;


	std::random_device rd;
	randEngine_.reset(new std::default_random_engine(rd()));
	std::uniform_int_distribution<> randomWaveIndex(0, 1);

	const FTransform& t = randomWaveIndex(*randEngine_) == 0 ?
		boss->waveUpperSpawnPoint_->GetComponentTransform() : boss->waveUnderSpawnPoint_->GetComponentTransform();
	UWorld* w = boss->GetWorld();
	if (w) {
		auto* wave = w->SpawnActor<AEnemyWeaponBase>(boss->waveClass_->GetDefaultObject()->GetClass(), t);
		if (wave) {
			wave->SetLifeSpan(1.5);
			numWaves_++;
			waitingSec_ = 0;
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("null world"));
	}
	return EBTNodeResult::InProgress;
}

void UBossWaveShots::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (waitingSec_ > IntervalSec_) {
		if (numWaves_ >= MaxWaves_) {
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else {
			ABossBase* boss = Cast<ABossBase>(OwnerComp.GetAIOwner()->GetPawn());
			if (!boss) {
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			UWorld* w = boss->GetWorld();
			if (w) {
				std::uniform_int_distribution<> randomWaveIndex(0, 1);
				const FTransform& t = randomWaveIndex(*randEngine_) == 0 ?
					boss->waveUpperSpawnPoint_->GetComponentTransform() : boss->waveUnderSpawnPoint_->GetComponentTransform();
				auto* wave = w->SpawnActor<AEnemyWeaponBase>(boss->waveClass_->GetDefaultObject()->GetClass(), t);
				if (wave) {
					wave->SetLifeSpan(1.5);
					numWaves_++;
					waitingSec_ = 0;
				}
			}
		}
	}
	else {
		waitingSec_ += DeltaSeconds;
	}
}