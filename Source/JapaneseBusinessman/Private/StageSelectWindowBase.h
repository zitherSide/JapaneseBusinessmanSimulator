// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectWindowBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDecideDispatcher, int, stageIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCancelDispatcher);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNGDispatcher);
/**
 * 
 */
UCLASS()
class UStageSelectWindowBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = Controller)
		FDecideDispatcher decideDispatcher_;
	UPROPERTY(BlueprintAssignable, Category = Controller)
		FCancelDispatcher cancelDispatcher_;
	UPROPERTY(BlueprintAssignable, Category = Controller)
		FNGDispatcher ngDispatcher_;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class USizeBox* stageBox1_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class USizeBox* stageBox2_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class USizeBox* stageBox3_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
		class USizeBox* stageBox4_;

	bool Initialize() override;

	UFUNCTION(BlueprintCallable, Category = Stage)
		FText getStageName(int i) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Save)
		class UJapaneseBusinessmanSaveGame* saveGame_;

	UFUNCTION(BlueprintCallable, Category = UI)
		void ReFocus();

	UFUNCTION(BlueprintCallable, Category = UI)
		int getCursorPos() const;

	UFUNCTION(BlueprintCallable, Category = Stage)
		FName getLevelName() const;

private:
	int xPos_{ 0 };
	int yPos_{ 0 };
	bool xNeutraled{ true };
	bool yNeutraled{ true };

	static const int MaxStageNum = 4;
	static const int YSize = 2;


	void OnAxisV(float val);
	void OnAxisH(float val);
	void OnDecide();
	void OnCancel();

};
