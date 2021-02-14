// Fill out your copyright notice in the Description page of Project Settings.


#include "FindTargetLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "JapaneseBusinessman/Public/WalkRoundAIController.h"
#include "AIController.h"
#include "EnemyBase.h"

EBTNodeResult::Type UFindTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBase* p = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (p) {
		UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent();
		int index = bb->GetValueAsInt(destinationIndexKey_.SelectedKeyName);
		if (p->destinations_.Num() == 0) {
			//UE_LOG(LogTemp, Error, TEXT("No destinations"));
			return EBTNodeResult::Failed;
		}

		index = ++index % p->destinations_.Num();
		bb->SetValueAsInt(destinationIndexKey_.SelectedKeyName, index);
		bb->SetValueAsVector(destinationKey_.SelectedKeyName, p->destinations_[index]->GetActorLocation());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Couldn't cast to emenybase"));
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
