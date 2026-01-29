// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void AInGameHUD::BeginPlay()
{
	// WidgetBlueprintのClassを取得する
	FString PauseWidgetPath = TEXT("/Game/UI/BluePrint/BP_PauseWidget.BP_PauseWidget_C");
	TSubclassOf<UUserWidget> PauseWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*PauseWidgetPath)).LoadSynchronous();

	// PlayerControllerを取得する
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PauseWidgetClass && PlayerController)
	{
		// Status表示用のWidgetを作成する
		PauseWidget = UWidgetBlueprintLibrary::Create(GetWorld(), PauseWidgetClass, PlayerController);

		// Pauseメニューを折りたたみ状態にする
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);

		// Viewportに追加する
		PauseWidget->AddToViewport(1);
	}

}

void AInGameHUD::DispPause(const bool IsPause)
{
	// PlayerControllerを取得する
	APlayerController* PlayerController = GetOwningPlayerController();

	if (IsPause)
	{
		// Pauseメニューを表示する
		PauseWidget->SetVisibility(ESlateVisibility::Visible);

		// UIモードに設定する
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, PauseWidget, EMouseLockMode::DoNotLock, false);

		// GameをPause状態にする
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		// Mouseカーソルを表示する
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		// Pause状態を解除する

		// GameのPause状態を解除する
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		// Mouseカーソルを非表示にする
		PlayerController->SetShowMouseCursor(false);

		// GameのInput状態に戻す
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, false);

		// Pauseメニューを折りたたみ状態にする
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void AInGameHUD::OpenLevel(const FName LevelName)
{
	// Level��Load����
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void AInGameHUD::QuitGame()
{
	// �Q�[����I������
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayerController(), EQuitPreference::Quit, false);
}