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

protected:
	virtual void BeginPlay() override;

public:
	// Pause���j���[�̕\��/��\��
	void DispPause(bool IsPause);

	// Level��J��
	void OpenLevel(FName LevelName);

	// Game��I������
	void QuitGame();

private:
	// PauseWidget��ێ�����ϐ�
	UUserWidget* PauseWidget;
};