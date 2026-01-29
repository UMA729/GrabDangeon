// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERCATCHER_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonResume;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonTitle;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonQuit;

protected:
	// NativeConstruct
	void NativeConstruct() override;

private:
	// ButtonResumeのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonResumeClicked();

	// ButtonTitleのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonTitleClicked();

	// ButtonQuitのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonQuitClicked();
};
