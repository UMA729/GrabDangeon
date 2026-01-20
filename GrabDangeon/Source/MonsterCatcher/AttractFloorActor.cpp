// Fill out your copyright notice in the Description page of Project Settings.


#include "AttractFloorActor.h"
#include "TimerManager.h" // 絶対必要

// Sets default values
AAttractFloorActor::AAttractFloorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttractHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttractHnadle"));
	AttractHandle->SetupAttachment(RootComponent);

	AttractObj = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttractObj"));
	AttractObj->SetupAttachment(AttractHandle);

	move_speed = 100.0f;

	// プレイヤーを乗せても滑らないようにする
	AttractObj->SetMobility(EComponentMobility::Movable);
	AttractObj->SetSimulatePhysics(false);
	AttractObj->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void AAttractFloorActor::BeginPlay()
{
	Super::BeginPlay();
	
	ori_pos = GetActorLocation();
	EndLocation = false;
}

// Called every frame
void AAttractFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isMoving) return;

	FVector CurrentLocation = GetActorLocation();
	FVector WorldMoveOffset = GetActorTransform().TransformVector(MoveOffset);
	FVector TargetLocation = ori_pos + WorldMoveOffset;

	if (!EndLocation)
	{
		FVector MoveDir = WorldMoveOffset.GetSafeNormal();
		CurrentLocation += MoveDir * (move_speed * 2.f) * DeltaTime;
		if (FVector::Dist(CurrentLocation, ori_pos) >= WorldMoveOffset.Size())
		{
			CurrentLocation = TargetLocation;
			EndLocation = true;

			StartWait();
		}
	}
	else
	{
		FVector MoveDir = (-WorldMoveOffset).GetSafeNormal();
		CurrentLocation += MoveDir * move_speed * DeltaTime;

		if (FVector::Dist(CurrentLocation, TargetLocation) >= WorldMoveOffset.Size())
		{
			CurrentLocation = ori_pos;
			EndLocation = false;

			isMoving = false;

		}
	}

	SetActorLocation(CurrentLocation,true);
	
}

void AAttractFloorActor::StartAttract()
{
	isMoving = true;
}

void AAttractFloorActor::StartWait()
{
	isMoving = false;

	GetWorld()->GetTimerManager().SetTimer(
		WaitTimerHandle,
		this,
		&AAttractFloorActor::ResumeMove,
		back_time,
		false
	);
}

void AAttractFloorActor::ResumeMove()
{
	isMoving = true;
}



