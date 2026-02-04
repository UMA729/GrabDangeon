// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "DoorActor.h"
#include "GoalActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UBoxComponent;

UCLASS()
class MONSTERCATCHER_API AGoalActor : public AActor
{
	GENERATED_BODY()
	
	bool isGoal;
public:	
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	AGoalActor();
	/** �J�ڂ���Level��Soft�Q�� */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TSoftObjectPtr<UWorld> LoadLevel;
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool isGimmickGoal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundBase* ButtonSound; //ボタンプッシュ音
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundBase* GoalSound; //ゴールオープン音

	UPROPERTY(EditAnywhere)
	ADoorActor* Door;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> GimmickActor;


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
	/** �ڐG����p��Collision : Sphere */
	UPROPERTY(VisibleAnywhere, Category = Goal, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Sphere;

	/** Goal�p��StaticMesh : Goal */
	UPROPERTY(VisibleAnywhere, Category = Goal, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Goal;


	// ���V�p�����[�^
	UPROPERTY(EditAnywhere, Category = "Floating")
	float FloatAmplitude = 20.0f;  // �㉺�̕��i�����j

	UPROPERTY(EditAnywhere, Category = "Floating")
	float FloatSpeed = 2.0f;       // �㉺�̑���

	// ��]�p�����[�^
	UPROPERTY(EditAnywhere, Category = "Floating")
	float RotationSpeed = 45.0f;   // 1�b������̉�]�p�x�i�x�j

	TArray<FVector> ActorIndex;
};
