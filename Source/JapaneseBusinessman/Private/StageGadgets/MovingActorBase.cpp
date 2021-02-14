// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingActorBase.h"
#include <Engine/TargetPoint.h>
#include "../../PlayerBase.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>

using namespace std;
// Sets default values
AMovingActorBase::AMovingActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	box_ = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box_->AttachTo(RootComponent);

	mesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh_->SetupAttachment(RootComponent);


	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovingActorBase::BeginPlay()
{
	Super::BeginPlay();
	mesh_->MoveIgnoreActors.Add(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (offsetTime_ > 0) {
		isWaiting_ = true;
		GetWorldTimerManager().SetTimer(waitTimer_, this, &AMovingActorBase::clearIsWaiting, offsetTime_, false);
	}
}

// Called every frame
void AMovingActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (destinations_.Num() == 0) {
		return;
	}

	if (isWaiting_)
		return;

	const FVector p = GetActorLocation();
	const FVector d = destinations_[nextDestination_]->GetActorLocation();

	if ((p - d).Size() < destMargin_) {
		SetActorLocation(d, true);
		nextDestination_++;
		nextDestination_ %= destinations_.Num();

		if (waitTime_ > 0) {
			isWaiting_ = true;
			GetWorldTimerManager().SetTimer(waitTimer_, this, &AMovingActorBase::clearIsWaiting, waitTime_, false);
		}
	}
	else {
		FVector velDir = d - p;
		velDir.Normalize();
		const FVector curPos = p + DeltaTime * velDir * speed_;
		FHitResult hitRes;
		const bool canMove = SetActorLocation(curPos, true);
		TArray<AActor*> others;
		GetOverlappingActors(others, TSubclassOf<APlayerBase>());
		//UE_LOG(LogTemp, Log, TEXT("overlap num: %d"), others.Num());
		std::for_each(others.begin(), others.end(), [this](AActor* a) {

			if (a->ActorHasTag(TEXT("Player"))) {
				auto* p = Cast<APlayerBase>(a);
				if (!p) return;

				const FVector aPos = p->GetActorLocation();
				const float halfHeight = p->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				const float pBtm = aPos.Z - halfHeight + crushMargin_;
				const float pTop = aPos.Z + halfHeight - crushMargin_;

				const FVector bPos = GetActorLocation();
				const FVector extent = box_->GetScaledBoxExtent();
				const float bBtm = bPos.Z - extent.Z;
				const float bTop = bPos.Z + extent.Z;
					
				const bool topInRange = pTop > bBtm && pTop < bTop;
				const bool btmInRage = pBtm > bBtm && pBtm < bTop;
				if (topInRange || btmInRage)
					UGameplayStatics::ApplyDamage(p, 9999, nullptr, this, TSubclassOf<UDamageType>());
			}
		});
	}
}

