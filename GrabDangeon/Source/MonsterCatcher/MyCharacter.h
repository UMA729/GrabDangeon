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
	TSubclassOf<AThrowKnifeActor>KnifeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* FireMontage;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	/**�O�l�̎��_�p�J����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCamera;

	/**��l�̎��_�p�J����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;

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

	//効果音---------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	USoundBase* RopeSound;//ロープ発射音
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	USoundBase* KnifeSound;//ナイフ発射音
	//---------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	class UCableComponent*GrappleCable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	bool isGrappling;	//�O���b�v���t���O
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	bool bIsFiringGrapple; // �P�[�u����L�΂��Ă���Œ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	bool isAttacking;

	UPROPERTY()
	USceneComponent* CableStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrappleAnchor;

	bool isPers;		//���_�t���O
	bool isRunning;		//�_�b�V���t���O

	// Grapple���
	FVector GrappleStart;  // ���C���ˊJ�n�ʒu
	FVector CableStart;		//�P�[�u�����ˈʒu
	FVector GrappleDir;    // �P�[�u�����˕���
	FVector GrabPoint;
	float InitialCableLength;
	float CurrentCableLength; // �P�[�u���̌��ݒ���
	float TargetCableLength;  // �P�[�u���̖ڕW����
	float Distance;

	float HP;

	int32 key_count;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> UIWidgetClass;

	UUI_UserWidget* MainWidgetInstance;

	UMyGameInstance* GameInstance;

	// ���C�̌��݈ʒu�i�P�[�u����[�j
	FVector GrappleTip;

	// ���C������������
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