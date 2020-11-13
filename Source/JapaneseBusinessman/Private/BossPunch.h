// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include <Engine/EngineTypes.h>
#include "BossPunch.generated.h"


/**
 * 
 */
UCLASS()
class UBossPunch : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FBlackboardKeySelector destinationKey_;
private:
	const float waitTimeSec_ = 1.f;
	const float punchSpeed_ = 2000.f;
	const float stopTime_ = 1.f;
	float stopElapsed_ = 0.f;
	float waitElapsed_ = 0.f;
	ECollisionEnabled::Type prevCollision_;
	FVector destination_;
	FVector velocity_;

	enum Phase {
		Waiting,
		Punching,
		Stopping
	};
	Phase phase_ = Phase::Waiting;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
