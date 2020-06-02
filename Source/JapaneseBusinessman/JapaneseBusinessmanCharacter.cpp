// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "JapaneseBusinessmanCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Vector.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// AJapaneseBusinessmanCharacter

AJapaneseBusinessmanCharacter::AJapaneseBusinessmanCharacter()
{
	//// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//// set our turn rates for input
	//BaseTurnRate = 45.f;
	//BaseLookUpRate = 45.f;

	//// Don't rotate when the controller rotates. Let that just affect the camera.
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	//// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = runSpeed_;
	GetCharacterMovement()->JumpZVelocity = jumpSpeed_;
	//GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	PrimaryActorTick.bCanEverTick = true;
}

void AJapaneseBusinessmanCharacter::BeginPlay()
{
	Super::BeginPlay();
	state_ = None;
}

void AJapaneseBusinessmanCharacter::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
	adjustDashMovement();
	adjustJumpMovement();
}

float AJapaneseBusinessmanCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (state_ & KnockBacking)
		return 0.f;

	FVector bounceDir;
	bounceDir.Z = FGenericPlatformMath::Sin(knockBackAngle_ / 2.f / PI);
	bounceDir.Y = FGenericPlatformMath::Cos(knockBackAngle_ / 2.f / PI);
	if (GetMesh()->GetRightVector().Y > 0) {
		bounceDir.Y *= -1;
	}

	state_ |= KnockBacking;
	state_ &= ~Dashing;

	currentHealth_ -= static_cast<int>(DamageAmount);
	GetCharacterMovement()->Launch(bounceDir * knockBackForce_);
	GetWorldTimerManager().ClearTimer(knockBackTimerHandle_);
	GetWorldTimerManager().SetTimer(knockBackTimerHandle_, this, &AJapaneseBusinessmanCharacter::RecoverDamage, knockBackSeconds_);
	
	hudUpdateEvent_.Broadcast();
	return DamageAmount;
}

void AJapaneseBusinessmanCharacter::RecoverDamage()
{
	state_ &= ~KnockBacking;
}
//////////////////////////////////////////////////////////////////////////
// Input

void AJapaneseBusinessmanCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AJapaneseBusinessmanCharacter::StopJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AJapaneseBusinessmanCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJapaneseBusinessmanCharacter::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AJapaneseBusinessmanCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AJapaneseBusinessmanCharacter::StopSprint);

	if (canDash_) {
		PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AJapaneseBusinessmanCharacter::Dash);
		PlayerInputComponent->BindAction("Dash", IE_Released, this, &AJapaneseBusinessmanCharacter::StopDash);
	}

	if (canThrow_) {
		PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AJapaneseBusinessmanCharacter::Throw);
	}
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AJapaneseBusinessmanCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AJapaneseBusinessmanCharacter::SlideCamera);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AJapaneseBusinessmanCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AJapaneseBusinessmanCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AJapaneseBusinessmanCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AJapaneseBusinessmanCharacter::OnResetVR);
}

void AJapaneseBusinessmanCharacter::SlideCamera(float Value)
{
	if (Value != 0.f) {
		UE_LOG(LogTemp, Log, TEXT("slideCamera"));
		CameraBoom->AddRelativeLocation({ 0, 0, 10 * Value });
	}
}

void AJapaneseBusinessmanCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AJapaneseBusinessmanCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AJapaneseBusinessmanCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AJapaneseBusinessmanCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AJapaneseBusinessmanCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AJapaneseBusinessmanCharacter::MoveForward(float Value)
{
	if (state_ & KnockBacking)
		return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		adjustMeshRotation();
	}
}

void AJapaneseBusinessmanCharacter::MoveRight(float Value)
{
	if (state_ & KnockBacking)
		return;

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		adjustMeshRotation();
	}
}

void AJapaneseBusinessmanCharacter::adjustMeshRotation()
{
	if (Controller) {
		static const FVector Base{ 0, 1, 0 };	//yz平面の基準ベクトル
		FVector flatten = GetCharacterMovement()->Velocity;
		flatten.Z = 0;
		float dot = FVector::DotProduct(flatten.GetSafeNormal(), Base);
		float theta = FGenericPlatformMath::Acos(dot);
		FRotator rot{0, theta * 180.f / PI, 0};
		GetMesh()->SetWorldRotation(rot);
	}
}

void AJapaneseBusinessmanCharacter::Jump()
{
	if (state_ & KnockBacking)
		return;

	Super::Jump();
}
void AJapaneseBusinessmanCharacter::StopJump()
{
	if (Controller && GetCharacterMovement()->Velocity.Z > 0) {
		GetCharacterMovement()->Velocity.Z = 0;
	}
}

void AJapaneseBusinessmanCharacter::Sprint()
{
	if (!GetCharacterMovement()->IsFalling()) {
		GetCharacterMovement()->MaxWalkSpeed = sprintSpeed_;
		GetCharacterMovement()->JumpZVelocity = highJumpSpeed_;
		state_ |= Sprinting;
	}
}

void AJapaneseBusinessmanCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed_;
	GetCharacterMovement()->JumpZVelocity = jumpSpeed_;
	state_ &= ~Sprinting;
}

void AJapaneseBusinessmanCharacter::Dash()
{
	if (state_ & KnockBacking)
		return;

	const bool isDashing = state_ & Dashing;
	UCharacterMovementComponent* m = GetCharacterMovement();
	if (!isDashing && !m->IsFalling()) {

		state_ |= Dashing;
		m->JumpZVelocity = highJumpSpeed_;
		GetWorldTimerManager().ClearTimer(dashTimerHandle_);
		GetWorldTimerManager().SetTimer(dashTimerHandle_, this, &AJapaneseBusinessmanCharacter::StopDash, dashTime_);
	}
}

void AJapaneseBusinessmanCharacter::StopDash()
{
	//空中でのダッシュ中をどうするか
	state_ &= ~Dashing;
	GetCharacterMovement()->MaxWalkSpeed = state_ & Sprinting ? sprintSpeed_ : runSpeed_;
	GetCharacterMovement()->JumpZVelocity = state_ & Sprinting ? highJumpSpeed_ : jumpSpeed_;
}

void AJapaneseBusinessmanCharacter::adjustDashMovement()
{
	if (state_ & Dashing) {
		UCharacterMovementComponent* m = GetCharacterMovement();
		m->MaxWalkSpeed = dashSpeed_;

		FVector direction = m->Velocity;
		direction.Z = 0;	//水平速度をダッシュスピードにする
		if (direction.Size() != 0.f) {
			direction.Normalize();
		}
		else {
			if (GetMesh()->GetForwardVector().X > 0) {
				direction = { 0, 1, 0 };
			}
			else {
				direction = { 0, -1, 0 };
			}
		}
		AddMovementInput(direction, 1.0);

		FVector newSpeed = direction * dashSpeed_;
		newSpeed.Z = m->Velocity.Z;	//垂直速度はそのままにする
		m->Velocity = newSpeed;
	}
}

void AJapaneseBusinessmanCharacter::adjustJumpMovement()
{
	if (state_ & KnockBacking)
		return;

	UCharacterMovementComponent* m = GetCharacterMovement();
	if (m->IsFalling() && InputComponent->GetAxisValue("MoveRight") == 0.f) {
		m->Velocity.Y = 0;
	}
}

void AJapaneseBusinessmanCharacter::Throw()
{
	FTransform t;
	t.SetLocation(GetActorLocation());
	FRotator r;
	r.Yaw = GetMesh()->GetRightVector().Y > 0 ? 90.f : -90.f;
	t.SetRotation(r.Quaternion());
	AProjectileBase* p = GetWorld()->SpawnActor<AProjectileBase>(throwObject_->GetDefaultObject()->GetClass(), t);
}