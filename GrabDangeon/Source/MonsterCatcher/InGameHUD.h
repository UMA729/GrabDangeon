// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERCATCHER_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// Pauseメニューの表示/非表示
	void DispPause(bool IsPause);

	// Levelを開く
	void OpenLevel(FName LevelName);

	// Gameを終了する
	void QuitGame();

private:
	// PauseWidgetを保持する変数
	UUserWidget* PauseWidget;
};