// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "GimmickWall.h"
#include "MyCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// RootComponent を作る
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);

	Mesh->SetGenerateOverlapEvents(true);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();


	//user_widget->SetKeyCount(GameInstance->keycount);
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//アイテム回転挙動
	FRotator ItemRatation = GetActorRotation();
	ItemRatation.Yaw += RotationSpeed * DeltaTime;
	SetActorRotation(ItemRatation);

	//アイテム上下移動
	FVector ItemLocation = StartLocation;
	float Time = GetWorld()->GetTimeSeconds();
	ItemLocation.Z += FMath::Sin(Time * FloatSpeed) * FloatAmplitude;
	SetActorLocation(ItemLocation);
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (AMyCharacter* character = Cast<AMyCharacter>(OtherActor))
	{

		character->KeyAdd();

		// インベントリ追加とかここに…

		Destroy();
	}

}