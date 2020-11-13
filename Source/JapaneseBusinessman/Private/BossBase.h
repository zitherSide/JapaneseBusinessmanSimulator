// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossBase.generated.h"

DECLARE_DYNAMIC_DELEGATE(FStartInvincible);
DECLARE_DYNAMIC_DELEGATE(FEndInvincible);
DECLARE_DYNAMIC_DELEGATE(FDie);

UCLASS()
class ABossBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float health_ = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Status)
		float maxHealth_ = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Sound)
		class UAudioComponent* chargeSound_;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Action)
		float chargeDurationSec_ = 1.0f;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = Status)
		float getHealth() { return health_; }
	UFUNCTION(BlueprintPure, Category = Status)
		float getHealthRate() { return health_ / maxHealth_; }

	UFUNCTION(BlueprintCallable, Category = Action)
		void InitialRoar();
	UPROPERTY(BlueprintReadWrite, Category = Action)
		bool isRoaring_ = false;

	UFUNCTION(BlueprintCallable, Category = Action)
		void Bounce();
	UFUNCTION(BlueprintCallable, Category = Action)
		void EndBounce();
	UPROPERTY(BlueprintReadWrite, Category = Action)
		bool isBouncing_ = false;

	UFUNCTION(BlueprintCallable, Category = Action)
		void Punch() { isPunching_ = true; }
	UPROPERTY(BlueprintReadWrite, Category = Action)
		bool isPunching_ = false;
	UFUNCTION(BlueprintCallable, Category = Action)
		void EndPunch() { isPunching_ = false; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action)
		bool isInvincible_ = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action)
		float invincibleTimeSec = 1.f;

	UPROPERTY(BlueprintReadWrite, Category = Action)
		class ATargetPoint* bounceEdge_;

	UPROPERTY(BlueprintReadWrite, Category = Action)
		class ATargetPoint* bounceGoal_;

	UPROPERTY(BlueprintReadWrite, Category = Action)
		FStartInvincible startInvincibleDelegate_;

	UPROPERTY(BlueprintReadWrite, Category = Action)
		FEndInvincible endInvincibleDelegate_;

	UPROPERTY(BlueprintReadWrite, Category = Event)
		FDie dieDelegate_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action)
		float bodyHitDamage_ = 10.f;

	UPROPERTY(VisibleAnywhere, Category = Action)
		class UArrowComponent* waveUnderSpawnPoint_;
	UPROPERTY(VisibleAnywhere, Category = Action)
		class UArrowComponent* waveUpperSpawnPoint_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action)
		TSubclassOf<class AEnemyWeaponBase> waveClass_;

private:
	FTimerHandle hTimer_;
	FTimerHandle hHitInvincibleTimer_;

	void endInvincible() { 
		isInvincible_ = false;
		endInvincibleDelegate_.ExecuteIfBound();
	}

	static const float WaitBodyHitDamage_;
	static const float BounceBodyHitDamage_;

};
