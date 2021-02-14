// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Templates/Subclassof.h>
#include "NPCBase.generated.h"

UCLASS()
class ANPCBase : public ACharacter
{
	GENERATED_BODY()
public:
	ANPCBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Talk, meta = (MakeEditWidget = true))
		FVector popUpPos_;

protected:
	// Sets default values for this character's properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Talk)
		class UAnimMontage* talkingAnim_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Talk)
		TSubclassOf<UUserWidget> popUpType_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Talk)
		class UUserWidget* popUp_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Talk)
		FText popUpContent_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Talk)
		class UBoxComponent* popUpRegion_;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
