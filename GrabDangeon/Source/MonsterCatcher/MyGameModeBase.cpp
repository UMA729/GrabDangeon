// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "EnemyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "InGameHUD.h"
#include "InGamePlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<AMyCharacter>PlayerBPClass(TEXT("/Game/BluePrints/Character/BP_MyCharacter"));

	if (PlayerBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerBPClass.Class;
		PlayerClass = PlayerBPClass.Class;
	}
	HUDClass = AInGameHUD::StaticClass();
	PlayerControllerClass = AInGamePlayerController::StaticClass();
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}

	const APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	if (PlayerStart)
	{
		SpawnTransform = PlayerStart->GetActorTransform();
	}
}

void AMyGameModeBase::KillPlayer(AMyCharacter* Player)
{

	//Player��j��
	Player->Destroy();

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AEnemyCharacter::StaticClass(),
		Enemies
	);

	for (AActor* Actor : Enemies)
	{
		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor))
		{
			Enemy->Reset();
		}
	}


	//Respawn��s��
	RespawnPlayer();
}

void AMyGameModeBase::RespawnPlayer()
{
	if (!PlayerClass) return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	// BP�N���X���琶��
	AMyCharacter* NewPlayer = GetWorld()->SpawnActor<AMyCharacter>(PlayerClass, SpawnTransform, SpawnInfo);
	if (NewPlayer)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->Possess(NewPlayer);
		}
	}
}