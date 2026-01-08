// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingActor.generated.h"

UCLASS()
class MONSTERCATCHER_API AMovingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingActor();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Platform")
	UStaticMeshComponent* PlatformMesh;

	// 移動量
	UPROPERTY(EditAnywhere, Category = "Platform")
	FVector MoveOffset = FVector(0, 500, 0);

	// 移動スピード
	UPROPERTY(EditAnywhere, Category = "Platform")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WaitTime = 2.0f; // 2秒停止など


private:
	FVector StartLocation;
	bool bGoingForward = true;

	FTimerHandle WaitTimerHandle;
	bool bIsWaiting = false;

	void StartWait();
	void ResumeMove();
};
