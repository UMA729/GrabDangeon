// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttractFloorActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class MONSTERCATCHER_API AAttractFloorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttractFloorActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttractHandle;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttractObj;

	UPROPERTY(EditAnywhere, Category = "Attract")
	float move_speed;

	UPROPERTY(EditAnywhere, Category = "Attract")
	FVector MoveOffset = FVector(0, 500, 0);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartAttract();
	void StartWait();
	void ResumeMove();

private:
	FVector ori_pos;
	FTimerHandle WaitTimerHandle;
	float back_time = 1.0f;
	bool EndLocation;
	bool isMoving;
};
