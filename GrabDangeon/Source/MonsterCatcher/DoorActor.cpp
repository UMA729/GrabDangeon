// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "DoorAnimInstance.h"

// Sets default values
ADoorActor::ADoorActor()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = RootScene;

	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADoorActor::OpenDoor()
{
	if (UAnimInstance* AnimInstance = DoorMesh->GetAnimInstance())
	{
		if (UDoorAnimInstance* DoorAnim = Cast<UDoorAnimInstance>(AnimInstance))
		{
			DoorAnim->bIsOpen = true;
		}
	}
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

