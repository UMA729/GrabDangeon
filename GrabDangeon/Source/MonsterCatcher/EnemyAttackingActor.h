// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAttackingActor.generated.h"

class USkeletalMeshComponent;
class USphereComponent;

UCLASS()
class MONSTERCATCHER_API AEnemyAttackingActor : public AActor
{
	GENERATED_BODY()
	

	//ÉÇÉfÉãÉÅÉbÉVÉÖ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BulletMesh;

	//çUåÇÇÃîªíË
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Action, meta = (AllowPrivateAccess = "true"))
	USphereComponent* AttackCollision;

	

public:	
	// Sets default values for this actor's properties
	AEnemyAttackingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
