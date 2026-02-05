// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();


	// ButtonPlay��OnClicked�ɁuOnButtonPlayClicked�v��֘A�Â���
	ButtonPlay->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnButtonPlayClicked);

	// ButtonQuit��OnClicked�ɁuOnButtonQuitClicked�v��֘A�Â���
	ButtonQuit->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnButtonQuitClicked);
}

void UMainMenuWidget::OnButtonPlayClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("play push"));


	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->Initialize();

	FName LevelName(*StageTutorial.GetAssetName());
	// Level01��Load����
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UMainMenuWidget::OnButtonQuitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("quit push"));
	// PlayerController��擾����
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// �Q�[����I������
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}
