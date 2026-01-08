// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "KillVolume.generated.h"

class UBoxComponent;

UCLASS()
class MONSTERCATCHER_API AKillVolume : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, Category = KillVolume, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = KillVolume, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> KillVolume;
	
	UFUNCTION()
	void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Sets default values for this actor's properties
	AKillVolume();

	UPROPERTY(EditAnywhere,Category = "KillVolume")
	FVector BoxExtent = FVector(32.0f, 32.0f, 32.0f);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform)override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
