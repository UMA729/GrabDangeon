// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GimmickWall.generated.h"

UCLASS()
class MONSTERCATCHER_API AGimmickWall : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AGimmickWall();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WallMesh;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartMoveUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

private:
	bool isMoving = false;
	bool isHit = false;
	float MoveSpeed = 200.f;
	float MoveDistance = 300.f;
	float MoveSoFar = 0.f;
};
