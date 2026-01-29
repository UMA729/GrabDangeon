// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingActor.h"
#include "TimerManager.h" // ��ΕK�v

// Sets default values
AMovingActor::AMovingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	SetRootComponent(PlatformMesh);

	// �v���C���[��悹�Ă����Ȃ��悤�ɂ���
	PlatformMesh->SetMobility(EComponentMobility::Movable);
	PlatformMesh->SetSimulatePhysics(false);
	PlatformMesh->SetEnableGravity(false);

}

// Called when the game starts or when spawned
void AMovingActor::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();

}

// Called every frame
void AMovingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// ��~���Ȃ瓮�����Ȃ�
	if (bIsWaiting) return;


	FVector CurrentLocation = GetActorLocation();
	FVector WorldMoveOffset = GetActorTransform().TransformVector(MoveOffset);
	FVector TargetLocation = StartLocation + WorldMoveOffset;

	// �ړ���������
	if (bGoingForward)
	{
		FVector MoveDir = WorldMoveOffset.GetSafeNormal();
		CurrentLocation += MoveDir * MoveSpeed * DeltaTime;

		if (FVector::Dist(CurrentLocation, StartLocation) >= WorldMoveOffset.Size())
		{
			CurrentLocation = TargetLocation;
			bGoingForward = false;

			// ��~�J�n
			StartWait();
		}
	}
	else
	{
		FVector MoveDir = (- WorldMoveOffset).GetSafeNormal();
		CurrentLocation += MoveDir * MoveSpeed * DeltaTime;

		if (FVector::Dist(CurrentLocation, TargetLocation) >= WorldMoveOffset.Size())
		{
			CurrentLocation = StartLocation;
			bGoingForward = true;

			// ��~�J�n
			StartWait();
		}

	}
		SetActorLocation(CurrentLocation);
}

void AMovingActor::StartWait()
{
	bIsWaiting = true;

	GetWorld()->GetTimerManager().SetTimer(
		WaitTimerHandle,
		this,
		&AMovingActor::ResumeMove,
		WaitTime,
		false
	);
}

void AMovingActor::ResumeMove()
{
	bIsWaiting = false;
}
