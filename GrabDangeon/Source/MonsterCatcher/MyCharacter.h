// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowKnifeActor.h"
#include "MyGameInstance.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacter.generated.h"

class UUI_UserWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AThrowKnifeActor;
struct FInputActionValue;

UCLASS()
class MONSTERCATCHER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=knife)
	TSubclassOf<AThrowKnifeActor>KnifeClass;//ナイフクラス

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* RopeMontage;//ロープを投げるときのアニメモンタージュ

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;//カメラの長さなどを変えるために便利
	
	/**�O�l�̎��_�p�J����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCamera;//三人称カメラ

	/**��l�̎��_�p�J����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;//一人称カメラ

	//入力アクション(マッピング)------------------------
	/**Input Mapping Context*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	/**Run Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PersAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrappleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;
	//-----------------------------------------------------

	//効果音---------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundBase* RopeSound;//ロープ発射音
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundBase* ThrowSound;//ナイフ発射音
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundBase* BGM;//BGM
	//---------------------------------------

	//ロープの描画に使う
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	class UCableComponent*GrappleCable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	bool isGrappling;	//現在スイング中
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	bool bIsFiringGrapple; // 現在ロープケーブルを伸ばしている

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	bool isAttacking;	//攻撃中

	UPROPERTY()
	USceneComponent* CableStartPoint;//ケーブルが描写される地点

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrappleAnchor;//レイがぶつかった地点とロープを接続させる

	bool isPers;		//視点フラグ
	bool isRunning;		//走っているかどうか

	// Grappleの処理に使う
	FVector GrappleStart;	// レイ発射開始地点
	FVector CableStart;		// ロープが出る位置
	FVector GrappleDir;		// ロープが発射される方向
	FVector GrabPoint;		// ロープがついた位置を保存する(動く天井に追従させるために必要)
	float InitialCableLength; // 
	float CurrentCableLength; // 現在のロープケーブルの長さ
	float TargetCableLength;  // ケーブルの目標への長さ
	float Distance;			  // ロープの縮む長さ

	float HP;				//主人公HP

	int32 key_count;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> UIWidgetClass;	//HPや鍵の数など

	UUI_UserWidget* MainWidgetInstance;	//UIウィジェットを保管する

	UMyGameInstance* GameInstance;		//ゲーム全体のステータス

	// レイの現在位置(ロープケーブルの先端)
	FVector GrappleTip;

	// ターゲットにヒットしているかどうか
	bool bHasHitTarget = false;

	void Damage(float damage);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Pers(const FInputActionValue& Value);

	void Run(const FInputActionValue& Value);
	void StopRun(const FInputActionValue& Value);

	void Grappling(const FInputActionValue& Value);
	void StopGrapple(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void KeyAdd();
	void KeySub();
};