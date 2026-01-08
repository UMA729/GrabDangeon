// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickWall.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

// Sets default values
AGimmickWall::AGimmickWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->OnComponentHit.AddDynamic(this, &AGimmickWall::OnHit);
	WallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallMesh->SetCollisionProfileName(TEXT("BlockAll"));

	RootComponent = WallMesh;
}

// Called when the game starts or when spawned
void AGimmickWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGimmickWall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!isHit)
	{
		if (AMyCharacter* character = Cast<AMyCharacter>(OtherActor))
		{
			UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			if (GameInstance->keycount > 0)
			{
				isHit = true;
				isMoving = true;
				character->KeySub();

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("don't have key"));
			}
		}
	}
}

// Called every frame
void AGimmickWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isMoving)
	{
		float MoveStep = MoveSpeed * DeltaTime;
		FVector WallLocation = GetActorLocation();
		WallLocation.Z += MoveStep;
		SetActorLocation(WallLocation);

		MoveSoFar += MoveStep;
		if (MoveSoFar >= MoveDistance)
		{
			isMoving = false;//è„è∏èIóπ
		}
	}
}

void AGimmickWall::StartMoveUp()
{
	isMoving = true;
	MoveSoFar = 0.f;
}