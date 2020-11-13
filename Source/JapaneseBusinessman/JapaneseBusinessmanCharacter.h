// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "Engine/EngineTypes.h"
#include <algorithm>
#include "Private/ProjectileBase.h"
#include "Templates/SubclassOf.h"
#include "JapaneseBusinessmanCharacter.generated.h"

UCLASS(config=Game)
class AJapaneseBusinessmanCharacter : public APlayerBase
{
	GENERATED_BODY()
public:
	AJapaneseBusinessmanCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, Category = Movement)
		float runSpeed_ = 600.f;
	UPROPERTY(EditAnywhere, Category = Movement)
		float jumpSpeed_ = 1200.f;

	UPROPERTY(EditAnywhere, Category = Movement)
		float sprintSpeed_ = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = Movement)
		float dashSpeed_ = 2000.f;

	UPROPERTY(EditAnywhere, Category = Movement)
		float dashTime_ = 0.3f;

	UPROPERTY(EditAnywhere, Category = Movement)
		float knockBackForce_ = 40000.f;
	UPROPERTY(EditAnywhere, Category = Movement)
		float knockBackAngle_ = 30.f;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
		bool isKnockBacking_ = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float knockBackSeconds_ = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		class UArrowComponent* throwDirection_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float attackLifespan_ = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		class UArrowComponent* knockBackDirection_;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = Skill)
		bool canThrow_ = true;
	UPROPERTY(EditAnywhere, Category = Skill)
		bool canDash_ = true;
	UPROPERTY(EditAnywhere, Category = Skill)
		bool canWallJump_ = true;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
		TSubclassOf<AProjectileBase> throwObject_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float standHight_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float dashHight_;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	virtual void Jump() override;
	void StopJump();
	void Sprint();
	void StopSprint();
	void Dash();
	void StopDash();
	void Throw();
	
	void YawCamera(float Value);
	void PitchCamera(float Value);
	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
private:
	FTimerHandle dashTimerHandle_;
	FTimerHandle knockBackTimerHandle_;
	FVector knockBackingDir_;

	void adjustMeshRotation();
	void adjustDashMovement();
	void adjustJumpMovement();

	bool isSprinting_ = false;
	bool isDashing_ = false;
	

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		bool isDashing() const { return isDashing_; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		bool isKnockBacking() const { return isKnockBacking_; }

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void RecoverDamage();
};

