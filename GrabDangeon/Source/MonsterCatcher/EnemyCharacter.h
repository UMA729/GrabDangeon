// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.generated.h"

class USphereComponent;

UCLASS()
class MONSTERCATCHER_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SightRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float LoseSightRadius = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SightAngle = 360.f;

	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float attack_radius;
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float stop_distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AttackMontage;

	//アイテムアクター
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	TSubclassOf<AItemActor> DropItemClass;

	//敵：ハイエナ
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool isHyena;
	//敵：始祖鳥
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool isArcheop;
	//敵：飛行キャラ（未定）
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool isFly_enemy;
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool gimmick_enemy;

	//攻撃フラグ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool bisAttacking;
	//飛行フラグ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool bisFlying;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	bool bisSpawn = true;

	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float HP;

	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float KnockbackPower = 600.0f;
	// Sets default values for this character's properties

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay()override;

private:
	UFUNCTION()
	void OnHitOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/** 接触判定用のCollision : Sphere */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Goal, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Sphere;

	//移動速度
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float move_speed;
	//攻撃クールダウン
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float cooldown_time;
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float attack_damage;
	
public:	
	APawn* my_pawn;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(float Deltatime);

	//ハイエナ挙動
	void Hyena(float Deltatime);
	//始祖鳥挙動
	void Archeop();
	//飛行キャラ挙動
	void FlyEnemy();
	//敵：攻撃
	void Attack();
	//モンタージュ待ち
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	//敵：攻撃ストップ
	void ResetAttack();
	//敵：ダッシュ;
	void Run();
	//敵：死亡
	void Die();

	void Reset();

	float rotate_speed;

	bool isLooking;
	bool isRush;
	bool isTargeting;
	bool isGround;

	FTimerHandle timer_handle;

	AActor*player_actor;

	FVector ori_pos;

};
