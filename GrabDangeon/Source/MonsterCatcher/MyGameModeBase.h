// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "EnemyCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERCATCHER_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AMyCharacter> PlayerClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	
	//コンストラクタ
	AMyGameModeBase();
	//リスポーン位置情報
	FTransform SpawnTransform;
	//Playerを破棄
	void KillPlayer(AMyCharacter* Player);


private:
	/** PlayerをRespawnする **/
	void RespawnPlayer();
};
