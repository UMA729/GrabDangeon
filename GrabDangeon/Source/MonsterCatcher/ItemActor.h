// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI_UserWidget.h"
#include "MyGameInstance.h"
#include "ItemActor.generated.h"

UCLASS()
class MONSTERCATCHER_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

	FVector StartLocation;

	// 壁の参照
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetWall;

private:
	UPROPERTY(VisibleAnywhere, Category = KillVolume, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// 浮遊パラメータ
	UPROPERTY(EditAnywhere, Category = "Floating")
	float FloatAmplitude = 20.0f;  // 上下の幅（高さ）

	UPROPERTY(EditAnywhere, Category = "Floating")
	float FloatSpeed = 2.0f;       // 上下の速さ

	// 回転パラメータ
	UPROPERTY(EditAnywhere, Category = "Floating")
	float RotationSpeed = 45.0f;   // 1秒あたりの回転角度（度）


};
