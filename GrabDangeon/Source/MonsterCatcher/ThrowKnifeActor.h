// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowKnifeActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MONSTERCATCHER_API AThrowKnifeActor : public AActor
{
	GENERATED_BODY()

	//ìäÇƒÇ´ïêäÌìñÇΩÇËîªíË
	UPROPERTY(VisibleAnywhere, Category = ThrowWeapon, meta = (AllowPrivateAccess = "true"))
	USphereComponent* KunaiCollision;
	//
	UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

private:
	float knife_damage;


public:	
	AThrowKnifeActor();
	// Sets default values for this actor's properties

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	USphereComponent* GetCollisionComp()const { return KunaiCollision; }
	
	UProjectileMovementComponent* GetProjectileMovement()const { return ProjectileMovement; }
};
