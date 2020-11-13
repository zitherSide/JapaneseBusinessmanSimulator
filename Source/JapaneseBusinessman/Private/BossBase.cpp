// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBase.h"
#include <Components/AudioComponent.h>
#include <Components/ArrowComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <algorithm>
#include <Blueprint/AIBlueprintHelperLibrary.h>

using namespace std;

const float ABossBase::WaitBodyHitDamage_ = 10.f;
const float ABossBase::BounceBodyHitDamage_ = 30.f;
// Sets default values
ABossBase::ABossBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	chargeSound_ = CreateDefaultSubobject<UAudioComponent>(TEXT("ChargeSound"));
	chargeSound_->SetupAttachment(RootComponent);
	chargeSound_->bAutoActivate = false;

	waveUnderSpawnPoint_ = CreateDefaultSubobject<UArrowComponent>(TEXT("WaveUnderSpawnPoint"));
	waveUnderSpawnPoint_->SetupAttachment(RootComponent);
	waveUpperSpawnPoint_ = CreateDefaultSubobject<UArrowComponent>(TEXT("WaveUpperSpawnPoint"));
	waveUpperSpawnPoint_->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABossBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossBase::InitialRoar()
{
	constexpr int steps = 20;
	const float quantum = maxHealth_ / steps;
	const float delay = chargeDurationSec_ / steps;
	isRoaring_ = true;

	health_ += quantum;
	if (chargeSound_) {
		chargeSound_->Play();
	}

	if (health_ < maxHealth_) {
		GetWorldTimerManager().ClearTimer(hTimer_);
		GetWorldTimerManager().SetTimer(hTimer_, this, &ABossBase::InitialRoar, delay, false);
	}
	else {
		isRoaring_ = false;
	}

}

void ABossBase::Bounce()
{
	isBouncing_ = true;
	bodyHitDamage_ = BounceBodyHitDamage_;
}

void ABossBase::EndBounce()
{
	isBouncing_ = false;
	bodyHitDamage_ = WaitBodyHitDamage_;
}

float ABossBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (isInvincible_ || health_ <= 0) {
		return 0;
	}

	isInvincible_ = true;
	startInvincibleDelegate_.ExecuteIfBound();
	health_ = max(0.f, health_ - DamageAmount);

	if (health_ <= 0) {
		dieDelegate_.ExecuteIfBound();
		GetCharacterMovement()->DisableMovement();
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
	}
	GetWorldTimerManager().ClearTimer(hHitInvincibleTimer_);
	GetWorldTimerManager().SetTimer(hHitInvincibleTimer_, this, &ABossBase::endInvincible, invincibleTimeSec, false);

	return DamageAmount;
}
