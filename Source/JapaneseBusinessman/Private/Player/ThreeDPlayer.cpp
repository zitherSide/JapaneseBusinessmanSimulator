// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreeDPlayer.h"
#include <algorithm>
#include <Components/InputComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <TimerManager.h>
#include <Kismet/GameplayStatics.h>

using namespace std;
static constexpr float RotAdjustThreshold = 1.0;

AThreeDPlayer::AThreeDPlayer()
{
	cameraBoom_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom_->SetupAttachment(RootComponent);

	camera_ = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera_->SetupAttachment(cameraBoom_);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AThreeDPlayer::BeginPlay()
{
	Super::BeginPlay();
	meshRotOffset_ = GetMesh()->GetRelativeRotation();
}

void AThreeDPlayer::SetupPlayerInputComponent(UInputComponent* input) 
{
	input->BindAxis("MoveForward", this, &AThreeDPlayer::MoveForward);
	input->BindAxis("MoveRight", this, &AThreeDPlayer::Turn);
	input->BindAction("Jump", IE_Pressed, this, &AThreeDPlayer::Jump);
	input->BindAction("Jump", IE_Released, this, &AThreeDPlayer::StopJump);
	input->BindAction("Dash", IE_Pressed, this, &AThreeDPlayer::Dash);
	input->BindAction("Dash", IE_Released, this, &AThreeDPlayer::StopDash);
	input->BindAxis("TurnRate", this, &AThreeDPlayer::TurnCamera);
	input->BindAxis("LookupRate", this, &AThreeDPlayer::LookupCamera);
	input->BindAction("CameraReset", IE_Pressed, this, &AThreeDPlayer::SetCameraBehind);
}

void AThreeDPlayer::MoveForward(float val)
{
	
	AddMovementInput(cameraBoom_->GetForwardVector(), val * 100);
}

void AThreeDPlayer::Turn(float val)
{
	
	const FVector r = val > 0 ? cameraBoom_->GetRightVector() : -cameraBoom_->GetRightVector();
	AddMovementInput(r, abs(val * 100));
}

void AThreeDPlayer::TurnCamera(float val)
{
	FRotator rot(0, val * 3, 0);
	cameraBoom_->AddRelativeRotation(rot);
}

void AThreeDPlayer::LookupCamera(float val)
{
	FRotator rot(val * 3, 0, 0);
	cameraBoom_->AddRelativeRotation(rot);
}

void AThreeDPlayer::SetCameraBehind()
{
	FQuat q;
	GetMesh()->GetRelativeRotationFromWorld(q);
	cameraBoom_->SetWorldRotation(q);
}
void AThreeDPlayer::Jump()
{
	Super::Jump();
}

void AThreeDPlayer::StopJump()
{
	UCharacterMovementComponent* m = GetCharacterMovement();
	m->Velocity.Z = min(m->Velocity.Z, 0.f);
}

void AThreeDPlayer::Tick(float deltaSec)
{
	adjustDash();
	adjustRotation();
}

float AThreeDPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (isInvincible_) {
		return 0;
	}

	isInvincible_ = true;
	currentHealth_ -= DamageAmount;
	GetWorldTimerManager().ClearTimer(invincibleTimer_);
	GetWorldTimerManager().SetTimer(invincibleTimer_, this, &AThreeDPlayer::endInvincible, invincibleTime_);
	damageDelegate_.ExecuteIfBound();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), hitDilationRate_);


	return DamageAmount;
}

void AThreeDPlayer::endInvincible()
{
	isInvincible_ = false;
	endDamageDelegate_.ExecuteIfBound();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
}

void AThreeDPlayer::adjustRotation()
{
	const UCharacterMovementComponent* m = GetCharacterMovement();
	FVector v = m->Velocity;
	v.Z = 0;
	if (v.Size() > RotAdjustThreshold) {
		FVector now = GetMesh()->GetForwardVector();
		now.Normalize();
		v.Normalize();

		const FQuat q = FQuat::FindBetweenNormals(now, v);
		GetMesh()->AddWorldRotation(q);
		GetMesh()->AddWorldRotation(meshRotOffset_);
	}
}

void AThreeDPlayer::Dash()
{
	UCharacterMovementComponent* m = GetCharacterMovement();
	if (!m->IsFalling() && !isDashing_) {
		isDashing_ = true;
		GetWorldTimerManager().ClearTimer(dashTimer_);
		GetWorldTimerManager().SetTimer(dashTimer_, this, &AThreeDPlayer::StopDash, dashTime_);
		m->MaxWalkSpeed = dashSpeed_;
	}
}

void AThreeDPlayer::StopDash()
{
	UCharacterMovementComponent* m = GetCharacterMovement();
	isDashing_ = false;
	m->MaxWalkSpeed = runSpeed_;
}

void AThreeDPlayer::adjustDash()
{
	if (!isDashing_)
		return;

	UCharacterMovementComponent* m = GetCharacterMovement();
	if (m->IsFalling()) {
		return;
	}

	FVector hVel = m->Velocity;
	hVel.Z = 0;
	hVel.Normalize();
	hVel = dashSpeed_ * hVel;

	m->Velocity.X = hVel.X;
	m->Velocity.Y = hVel.Y;
}