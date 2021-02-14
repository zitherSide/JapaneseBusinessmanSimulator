// Fill out your copyright notice in the Description page of Project Settings.


#include "MoshpitEnemy.h"
#include <algorithm>
#include "../../PlayerBase.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>

using namespace std;
// Sets default values
AMoshpitEnemy::AMoshpitEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMoshpitEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoshpitEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMoshpitEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMoshpitEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const ACharacter* p = Cast<ACharacter>(DamageCauser);
	if (!p)
		return 0;

	FVector causerVel = p->GetCharacterMovement()->Velocity;
	const float causerSpeed = causerVel.Size();
	causerVel.Normalize();

	if (causerSpeed > smashThreshold_) {
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		causerVel.Z = max(causerVel.Z, 0.1f);
		GetCharacterMovement()->AddImpulse(causerVel * smashSpeed_, true);
		canDamage_ = false;
		GetCapsuleComponent()->SetCapsuleHalfHeight(0);
	}
	else {
		FVector dir = GetActorLocation() - p->GetActorLocation();
		dir.Normalize();
		AddMovementInput(dir, nudgeSpeed_, true);
	}

	return 0;
}

