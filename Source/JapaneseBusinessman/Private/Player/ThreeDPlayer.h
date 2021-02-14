// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PlayerBase.h"
#include "ThreeDPlayer.generated.h"


DECLARE_DYNAMIC_DELEGATE(FTakeDamage);
DECLARE_DYNAMIC_DELEGATE(FEndDamage);
/**
 * 
 */
UCLASS()
class AThreeDPlayer : public APlayerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
		class USpringArmComponent* cameraBoom_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
		class UCameraComponent* camera_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float invincibleTime_ = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float hitDilationRate_ = 0.5f;

	UFUNCTION(BlueprintCallable, Category = Movement)
		bool isInvincible() const { return isInvincible_; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FTakeDamage damageDelegate_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damege)
		FEndDamage endDamageDelegate_;
	AThreeDPlayer();

protected:
	void BeginPlay() override;
	virtual void Tick(float deltaSec) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float val);
	void Turn(float val);
	void TurnCamera(float val);
	void LookupCamera(float val);
	void SetCameraBehind();
	void Jump();
	void StopJump();
	void Dash();
	void StopDash();

private:
	FRotator meshRotOffset_;
	FTimerHandle dashTimer_;
	FTimerHandle invincibleTimer_;
	bool isInvincible_{ false };

	void adjustRotation();
	void adjustDash();
	void endInvincible();
};
