// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPunch.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <AIModule/Classes/AIController.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "BossBase.h"


EBTNodeResult::Type UBossPunch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;
	stopElapsed_ = 0.f;
	waitElapsed_ = 0.f;
	phase_ = Phase::Waiting;

	ABossBase* boss = Cast<ABossBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!boss)
		return EBTNodeResult::Failed;

	UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent();
	destination_ = bb->GetValueAsVector(destinationKey_.SelectedKeyName);
	prevCollision_ = boss->GetCapsuleComponent()->GetCollisionEnabled();
	boss->Punch();

	return EBTNodeResult::InProgress;
}

void UBossPunch::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ABossBase* boss = Cast<ABossBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!boss) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	switch (phase_) {
	case Phase::Waiting:
		stopElapsed_ += DeltaSeconds;
		if (stopElapsed_ >= waitTimeSec_) {
			velocity_ = destination_ - boss->GetActorLocation();
			velocity_.Normalize();
			velocity_ *= punchSpeed_;

			boss->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			boss->GetCharacterMovement()->DisableMovement();
			phase_ = Phase::Punching;
		}
		return;
	case Phase::Punching: {
		const bool reached = (velocity_.Y > 0 && boss->GetActorLocation().Y >= destination_.Y) ||
			(velocity_.Y <= 0 && boss->GetActorLocation().Y <= destination_.Y);
		if (reached) {
			phase_ = Phase::Stopping;
		}
		break;
	}
	case Phase::Stopping:
		boss->GetCapsuleComponent()->SetCollisionEnabled(prevCollision_);
		boss->EndPunch();
		boss->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		FVector pos = boss->GetActorLocation();
		pos.Z = std::max(5.f, pos.Z);
		boss->SetActorLocation(pos);	//–„‚à‚ê–hŽ~
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		break;
	}

	boss->SetActorLocation(boss->GetActorLocation() + velocity_ * DeltaSeconds, true);
}
