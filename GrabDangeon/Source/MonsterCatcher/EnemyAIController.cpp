// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "MyCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"   

AEnemyAIController::AEnemyAIController()
{

	PerceptionCmp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionCmp->ConfigureSense(*SightConfig);
	PerceptionCmp->SetDominantSense(SightConfig->GetSenseImplementation());

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (PerceptionCmp)
	{
		PerceptionCmp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	}
}

void AEnemyAIController::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (AMyCharacter* character = Cast<AMyCharacter>(Actor))
	{
		//AIControllerを使うEnemyCharacterクラスを持ってくる
		if (AEnemyCharacter* EnemyClass = Cast<AEnemyCharacter>(GetPawn()))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				//EnemyのMove関数で動くようにPlayerと自分の位置情報などが入ってるPawn、Actorを持っていく
				UE_LOG(LogTemp, Warning, TEXT("look"));
			
				EnemyClass->player_actor = Actor;
				EnemyClass->my_pawn = GetPawn();
				//見つかった判定
				EnemyClass->isLooking = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("leave：%s"), *Actor->GetName());

				EnemyClass->isLooking = false;
				EnemyClass->bisAttacking = false;
			}
		}
	}
}