// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalActor.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoalActor::AGoalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	GoalText = CreateDefaultSubobject<AActor>(TEXT("GoalText"));

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	// SphereCollision‚ğ’Ç‰Á‚·‚é
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	Sphere->SetupAttachment(RootComponent);
	// Radius‚ğİ’è‚·‚é
	Sphere->SetSphereRadius(72.0f);

	// OnComponentBeginOverlap‚ğBind‚·‚é
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGoalActor::OnSphereBeginOverlap);

	// StaticMeshComponent‚ğ’Ç‰Á‚µARootComponent‚Éİ’è‚·‚é
	Goal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Goal->SetupAttachment(RootComponent);

}

void AGoalActor::BeginPlay()
{
	Super::BeginPlay();

	if (!GoalText)
	{
		return;
	}

	if (isGimmickGoal&&GoalText)
		GoalText->SetActorHiddenInGame(true);
	else
		isGoal = true;

	ActorIndex.Empty();
	ActorIndex.Reserve(GimmickActor.Num());
	for (AActor * Gimmick:GimmickActor)
	{
		if (!Gimmick) continue;

		ActorIndex.Add(Gimmick->GetActorLocation());

		Gimmick->SetActorHiddenInGame(true);
		Gimmick->SetActorEnableCollision(false);
		Gimmick->SetActorTickEnabled(false);
	}

	StartLocation = GoalText->GetActorLocation();
	StartRotate = GoalText->GetActorRotation();
}

void AGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GoalText)
	{
		return;
	}

	if (GoalText && isGoal)
	{
		FRotator GoalRatation = StartRotate;
		float Time = GetWorld()->GetTimeSeconds();
		GoalRatation.Yaw += FMath::Sin(Time * FloatSpeed) * FloatAmplitude;
		GoalText->SetActorRotation(GoalRatation);

		FVector GoalLocation = StartLocation;
		GoalLocation.Z += FMath::Sin(Time * FloatSpeed) * FloatAmplitude;
		GoalText->SetActorLocation(GoalLocation);
	}
}

void AGoalActor::OpenGoal()
{
	UE_LOG(LogTemp, Warning, TEXT("‚Í‚¢‚è‚Ü‚µ‚½"));
	isGoal = true;
	GoalText->SetActorHiddenInGame(false);

	for (int32 i = 0; i < GimmickActor.Num(); ++i)
	{

		AActor* Gimmick = GimmickActor[i];

		if (!Gimmick) continue;

		Gimmick->SetActorLocation(ActorIndex[i]);
		Gimmick->SetActorHiddenInGame(false);
		Gimmick->SetActorEnableCollision(true);
		Gimmick->SetActorTickEnabled(true);
	}
}

void AGoalActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isGoal)
	{
		// ÚG‚µ‚½Actor‚ªBallPlayer‚©”»’è‚·‚é
		if (const AMyCharacter* player = Cast<AMyCharacter>(OtherActor))
		{
			// ‘JˆÚ‚·‚éLevel‚ğLoad‚·‚é
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, LoadLevel);
		}
	}
}