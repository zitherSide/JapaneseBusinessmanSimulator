// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include <Engine/EngineTypes.h>
#include <random>
#include <memory>
#include "BossWaveShots.generated.h"

/**
 * 
 */
UCLASS()
class UBossWaveShots : public UBTTaskNode
{
	GENERATED_BODY()
private:
	const float IntervalSec_ = 1.1f;
	float waitingSec_ = 0.f;

	const int MaxWaves_ = 5;
	int numWaves_ = 0;

	std::unique_ptr<std::default_random_engine> randEngine_;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FBlackboardKeySelector waveCountKey_;
};
