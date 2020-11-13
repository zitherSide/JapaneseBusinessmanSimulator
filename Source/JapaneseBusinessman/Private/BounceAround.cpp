// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceAround.h"
#include "BossBase.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <AIModule/Classes/AIController.h>
#include <Engine/TargetPoint.h>
#include <Components/CapsuleComponent.h>

EBTNodeResult::Type UBounceAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;
	isForth_ = true;

	ABossBase* boss = Cast<ABossBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!boss) 
		return EBTNodeResult::Failed;
	
	prevCollision_ = boss->GetCapsuleComponent()->GetCollisionEnabled();
	boss->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boss->Bounce();
	return EBTNodeResult::InProgress;
}

void UBounceAround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ABossBase* boss = Cast<ABossBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!boss) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UCharacterMovementComponent* m = boss->GetCharacterMovement();
	if (!m) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	m->DisableMovement();
	gravity_.Z = m->GetGravityZ() * 2;
	if (m->GetActorLocation().Z > 100) {
		velocity_ += gravity_ * DeltaSeconds;
	}
	else {
		FVector dir;
		if (isForth_) {
			dir = boss->bounceEdge_->GetActorLocation() - boss->GetActorLocation();
		}
		else {
			dir = boss->bounceGoal_->GetActorLocation() - boss->GetActorLocation();
		}
		
		dir.Normalize();
		velocity_ = dir * JumpSpeedH_;
		velocity_.Z = JumpSpeedV_;
	}

	if (isForth_ && boss->GetActorLocation().Y >= boss->bounceEdge_->GetActorLocation().Y) {
		isForth_ = false;
		velocity_.Y *= -1;
	}
	boss->SetActorLocation(boss->GetActorLocation() + velocity_ * DeltaSeconds, true);

	if (!isForth_ && boss->GetActorLocation().Y <= boss->bounceGoal_->GetActorLocation().Y) {
		FVector endPos;
		EMovementMode endMode;
		if (boss->GetActorLocation().Z > 10) {
			endMode = EMovementMode::MOVE_Falling;
			endPos.Y = boss->bounceGoal_->GetActorLocation().Y;
			m->Velocity = velocity_;
			m->Velocity.Y = 0;
		}
		else {
			endMode = EMovementMode::MOVE_Walking;
			endPos = boss->bounceGoal_->GetActorLocation();
			m->Velocity = {};
		}

		m->SetMovementMode(endMode);
		boss->SetActorLocation(endPos);
		boss->GetCapsuleComponent()->SetCollisionEnabled(prevCollision_);
		boss->EndBounce();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
