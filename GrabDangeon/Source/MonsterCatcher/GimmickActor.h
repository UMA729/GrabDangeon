// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GimmickActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class MONSTERCATCHER_API AGimmickActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGimmickActor();


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GimmickMesh;

	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool MovingFloor; 

	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool isSpawnDest;

	bool StartSpawn = false;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	int32 MaxActiveFloor = 3;

	int32 CurrentIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	TArray<AActor*> GimmickFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Gimmick;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	TArray<AActor*> EnemyActors;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	float SpawnTime = 1.0f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Emerge();

	void Appear(AActor*Floor);
	void Disappear(AActor* Floor);

	void StartGimmick();
	void UpdateGimmick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	bool isHit = false;
	float MoveSpeed = 200.f;
	float MoveDistance = 300.f;
	float MoveSoFar = 0.f;
	FVector ButtonStartLocation;
	TArray<FVector> EnemyStartLocations;
	TArray<FVector> FloorStartLocations;
	FTimerHandle WaitTimerHandle;
	TArray<AActor*> ActiveFloors;

};
