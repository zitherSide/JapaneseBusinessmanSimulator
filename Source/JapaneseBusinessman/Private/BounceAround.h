// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include <Engine/EngineTypes.h>
#include "BounceAround.generated.h"
/**
 * 
 */
UCLASS()
class UBounceAround : public UBTTaskNode
{
	GENERATED_BODY()
private:
	const float JumpSpeedV_ = 1300.f;
	const float JumpSpeedH_ = 700.f;
	FVector gravity_{ 0.f, 0.f, 0.f };
	FVector velocity_{ 0.f, 0.f, 0.f };
	bool isForth_ = true;
	ECollisionEnabled::Type prevCollision_;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
