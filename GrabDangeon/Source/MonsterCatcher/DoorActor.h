// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UCLASS()
class MONSTERCATCHER_API ADoorActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADoorActor();

	UPROPERTY(VisibleAnywhere, Category = DoorRoot, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* DoorMesh;

	void OpenDoor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
