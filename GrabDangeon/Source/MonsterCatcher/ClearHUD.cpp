// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void AClearHUD::BeginPlay()
{
	FString Path = TEXT("/Game/UI/Clear/BP_ClearWidget.BP_ClearWidget_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerController��擾����
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// WidgetClass��PlayerController���擾�ł��������肷��
	if (WidgetClass && PlayerController)
	{
		// Widget��쐬����
		UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		// Viewport�ɒǉ�����
		UserWidget->AddToViewport(0);

		// MouseCursor��\������
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UserWidget, EMouseLockMode::DoNotLock, true, false);
		PlayerController->SetShowMouseCursor(true);
	}
}

