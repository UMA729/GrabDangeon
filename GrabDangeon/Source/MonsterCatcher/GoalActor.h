// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GoalActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class MONSTERCATCHER_API AGoalActor : public AActor
{
	GENERATED_BODY()
	
	bool isGoal;
public:	
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	AGoalActor();
	/** 遷移するLevelのSoft参照 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TSoftObjectPtr<UWorld> LoadLevel;
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool isGimmickGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> GimmickActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* GoalText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* LeverActor;

	void OpenGoal();

	FVector StartLocation;
	FRotator StartRotate;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = KillVolume, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** 接触判定用のCollision : Sphere */
	UPROPERTY(VisibleAnywhere, Category = Goal, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Sphere;

	/** Goal用のStaticMesh : Goal */
	UPROPERTY(VisibleAnywhere, Category = Goal, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Goal;


	// 浮遊パラメータ
	UPROPERTY(EditAnywhere, Category = "Floating")
	float FloatAmplitude = 20.0f;  // 上下の幅（高さ）

	UPROPERTY(EditAnywhere, Category = "Floating")
	float FloatSpeed = 2.0f;       // 上下の速さ

	// 回転パラメータ
	UPROPERTY(EditAnywhere, Category = "Floating")
	float RotationSpeed = 45.0f;   // 1秒あたりの回転角度（度）

	TArray<FVector> ActorIndex;
};
