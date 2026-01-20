// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GimmickActor.h"
#include "AttractFloorActor.h"
#include "UI_UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CableComponent.h"
#include "DrawDebugHelpers.h"
#include "GoalActor.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	// カメラアームを作る (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// TPSカメラ
	//ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	//ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//ThirdPersonCamera->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// FPSカメラ
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(CameraBoom); // 頭のSocketにくっつける想定

	//グラップルケーブル
	GrappleCable = CreateDefaultSubobject<UCableComponent>(TEXT("GrappleCable"));
	GrappleCable->SetupAttachment(GetMesh(), TEXT("RightHand"));
	GrappleCable->SetVisibility(false);
	
	//GrappleCable->bEnableStiffness = true;	//張力を有効
	GrappleCable->bEnableCollision = true;		//衝突判定を切る
	GrappleCable->NumSegments = 10;
	GrappleCable->SolverIterations = 16;
	// ケーブルは全員に見せたい（特にプレイヤーにも）
	GrappleCable->SetOwnerNoSee(false);
	GrappleCable->SetOnlyOwnerSee(false);

	//isPers = true;
	isRunning = false;
	isGrappling = false;
	
	HP = 100;
	key_count = 0;
	Distance = 0.f;
}


// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();	

	GetMesh()->HideBoneByName(FName("head"), EPhysBodyOp::PBO_None);

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (UIWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIWiget"));
		MainWidgetInstance = CreateWidget<UUI_UserWidget>(GetWorld(), UIWidgetClass);
	
		if (MainWidgetInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Instance"));
			MainWidgetInstance->AddToViewport();
			MainWidgetInstance->SetKeyCount(GameInstance->keycount);
		}
	}
	/*if (isPers)
	{
		if (FirstPersonCamera && ThirdPersonCamera)
		{
			FirstPersonCamera->SetActive(true);
			ThirdPersonCamera->SetActive(false);
		}
	}
	else
	{
		if (FirstPersonCamera && ThirdPersonCamera)
		{
			FirstPersonCamera->SetActive(false);
			ThirdPersonCamera->SetActive(true);
		}
	}*/
	/*if (GrappleCable && FirstPersonCamera)
	{
		GrappleCable->AttachToComponent(FirstPersonCamera, FAttachmentTransformRules::KeepRelativeTransform);
		GrappleCable->SetRelativeLocation(FVector::ZeroVector);
		GrappleCable->SetRelativeRotation(FRotator::ZeroRotator);
	}*/
}

void AMyCharacter::Damage(float damage)
{
	HP -= damage;

	if (MainWidgetInstance)
	{
		MainWidgetInstance->SetHPPercent(HP/100.f);
	}
}


void AMyCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//毎フレーム更新
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// ケーブル飛翔中（伸ばし中）
		if (bIsFiringGrapple)
		{
			// 先端を伸ばす
			float FireSpeed = 4000.f;
			CurrentCableLength += FireSpeed * DeltaTime;

			//FVector AdjustedDir = GrappleDir;
			//AdjustedDir = FVector(0.f,0.f,0.f); // X成分をゼロにして無視
			//AdjustedDir.Normalize(); // 正規化し直す（重要）

			//ここでXが+されてしまうのが原因 == ×
			//GrappleTip = GrappleStart + GrappleDir * CurrentCableLength;

			//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), GrappleTip.X, GrappleTip.Y, GrappleTip.Z);
			// レイを都度飛ばす（先端まで）
			//グラップル天井用レイ
			bool gHit;
			FHitResult GraHit;
			FCollisionQueryParams GrappleParams;
			GrappleParams.AddIgnoredActor(this);
			FCollisionObjectQueryParams  GraObjParams;
			GraObjParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
			//ギミック用レイ
			bool bHit;
			FHitResult GimHit;
			FCollisionQueryParams GimmickParams;
			GimmickParams.AddIgnoredActor(this);
			FCollisionObjectQueryParams  GimObjParams;
			GimObjParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);




			GrappleTip = GrappleStart + GrappleDir * CurrentCableLength;

			gHit = GetWorld()->LineTraceSingleByObjectType(
				GraHit,
				GrappleStart,
				GrappleTip,
				GraObjParams,
				GrappleParams
			);

			bHit = GetWorld()->LineTraceSingleByObjectType(
				GimHit,
				GrappleStart,
				GrappleTip,
				GimObjParams,
				GimmickParams
			);

			//--------------追加--------------------------
			// 伸ばす予定の先端位置
			if (!bHit && !gHit)
			{

				FVector DesiredTip = GrappleStart + GrappleDir * CurrentCableLength;

				// 先にブロッカー判定（壁など）
				FHitResult BlockHit;
				bool bBlocked = GetWorld()->LineTraceSingleByChannel(
					BlockHit,
					GrappleStart,
					DesiredTip,
					ECC_Visibility // 壁・床が Block の TraceChannel
				);

				// ブロックされていたら、そこで止める
				if (bBlocked)
				{
					GrappleTip = BlockHit.Location; // 壁まで
					//グラップル停止
					bIsFiringGrapple = false;
					GrappleCable->SetVisibility(false);
					if (GrappleAnchor)
					{
						GrappleAnchor->DestroyComponent();
						GrappleAnchor = nullptr;
					}

					gHit = false;            // 天井にヒットしていない扱いに
					bHit = false;            // 天井にヒットしていない扱いに
					return;                         // ★ここで処理終了が重要！
				}
			}
			//---------------------------------------------

			if (bHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("HitActor: %s"), *GimHit.GetActor()->GetName());
			}

			// 視覚確認用ライン
			//DrawDebugLine(GetWorld(), GrappleStart, GrappleTip, FColor::Green, false, -1.0f, 0, 2.0f);

			// ケーブルの先端位置更新
			if (!gHit)
				GrappleCable->SetWorldLocation(GrappleStart);


			if (!bHasHitTarget)
			{

				if (GrappleCable)
				{
					// CableComponent自体の位置は「開始点(ソケット)」のままにしておき、終端アタッチ先（Anchor）を動かす方が一般的
					// Anchor が無ければ作成して FirstPersonCamera に相対アタッチしておく
					if (!GrappleAnchor)
					{
						GrappleAnchor = NewObject<USceneComponent>(this, TEXT("GrappleAnchor"));
						if (GrappleAnchor)
						{
							GrappleAnchor->RegisterComponent();
							GrappleAnchor->AttachToComponent(FirstPersonCamera, FAttachmentTransformRules::KeepRelativeTransform);
						}
					}

					if (GrappleAnchor)
					{
						// Anchor を先端位置に移動させ、ケーブルの終点を Anchor にアタッチ
						GrappleAnchor->SetWorldLocation(GrappleTip);
						GrappleCable->SetAttachEndToComponent(GrappleAnchor, NAME_None);
					}
				}

			}

			// 命中した瞬間
			if (bHit && !bHasHitTarget)
			{
				if (AActor* HitActor = GimHit.GetActor())
				{
					if (AGoalActor* LeverActor = Cast<AGoalActor>(HitActor))
					{
						LeverActor->OpenGoal();
					}
					else if (AGimmickActor* GimmickClass = Cast<AGimmickActor>(HitActor))
					{
						if (!GimmickClass->isSpawnDest)
							GimmickClass->Emerge();
						else if (GimmickClass->isSpawnDest)
							GimmickClass->StartGimmick();
					}
					else if (AAttractFloorActor* AttractClass = Cast<AAttractFloorActor>(HitActor))
					{
						AttractClass->StartAttract();
					}
				}

				bIsFiringGrapple = false;
				GrappleCable->SetVisibility(false);
				if (GrappleAnchor)
				{
					GrappleAnchor->DestroyComponent();
					GrappleAnchor = nullptr;
				}

				gHit = false;            // 天井にヒットしていない扱いに
				bHit = false;            // 天井にヒットしていない扱いに
				return;                  // ★ここで処理終了が重要！

			}
			if (gHit && !bHasHitTarget)
			{
				GrappleCable->SetWorldLocation(CableStart);

				bHasHitTarget = true;
				bIsFiringGrapple = false;
				isGrappling = true;

				// ヒットしたコンポーネント
				USceneComponent* HitComp = GraHit.GetComponent();

				if (HitComp && GrappleAnchor)
				{
					// いったんワールド位置を合わせる
					GrappleAnchor->SetWorldLocation(GraHit.ImpactPoint);

					// ★ここが核心：天井にアタッチ
					GrappleAnchor->AttachToComponent(
						HitComp,
						FAttachmentTransformRules::KeepWorldTransform
					);
				}

				// 初期ロープ長
				CurrentCableLength = FVector::Distance(
					GetActorLocation(),
					GrappleAnchor->GetComponentLocation()
				);

				TargetCableLength = CurrentCableLength;
				//UE_LOG(LogTemp, Warning, TEXT("Grapple Hit: %s"), *GrabPoint.ToString());
			}

			// 一定距離まで伸ばしたのに当たらなかったらリセット
			if (CurrentCableLength > 3000.f && !bHasHitTarget)
			{
				bIsFiringGrapple = false;
				GrappleCable->SetVisibility(false);
				if (GrappleAnchor)
				{
					GrappleAnchor->DestroyComponent();
					GrappleAnchor = nullptr;
				}
			}
		}


		// 振り子物理処理
		if (isGrappling)
		{
			CableStart = GetMesh()->GetSocketLocation(TEXT("RightHand"));
			GrappleCable->SetWorldLocation(CableStart);

			const float MinCableLength = 300.f;

			// ←今までは Max で強制的に縮めていたが、
			//    ここを滑らかな補間に変える
			float InterpSpeed = 500.f; // ← 速くしたいなら20〜30にしてOK

			CurrentCableLength = FMath::FInterpTo(
				CurrentCableLength,   // 現在の長さ
				MinCableLength,       // 目標の長さ（第二引数）
				DeltaTime,
				InterpSpeed
			);

			FVector ActorLoc = GetActorLocation();
			FVector AnchorLoc = GrappleAnchor->GetComponentLocation();
			FVector ToAnchor = AnchorLoc - ActorLoc;
			float DistanceToAnchor = ToAnchor.Size();
			FVector RopeDir = ToAnchor.GetSafeNormal();

			FVector CorrectedPos = AnchorLoc - RopeDir * CurrentCableLength;
			FVector Correction = CorrectedPos - ActorLoc;

			GetCharacterMovement()->AddForce(Correction * 900.f);
			GetCharacterMovement()->AddForce(FVector(0, 0, -980.f * GetCharacterMovement()->Mass));

			FVector Velocity = GetCharacterMovement()->Velocity;
			float SpeedAlongRope = FVector::DotProduct(Velocity, RopeDir);
			FVector TangentialVelocity = Velocity - RopeDir * SpeedAlongRope;
			GetCharacterMovement()->Velocity = TangentialVelocity;
		}


	// 振り子物理処理 実験
	//if (isGrappling)
	//{
	//	FVector ActorLoc = GetActorLocation();
	//	FVector ToAnchor = GrabPoint - ActorLoc;
	//	float dDistanceToAnchor = ToAnchor.Size();
	//	FVector RopeDir = ToAnchor.GetSafeNormal();

	//	// ロープの長さを常に現在の距離に追従させる
	//	CurrentCableLength = DistanceToAnchor;

	//	// ロープの張力補正（軽く引き戻すように）
	//	FVector CorrectedPos = GrabPoint - RopeDir * CurrentCableLength;
	//	FVector Correction = CorrectedPos - ActorLoc;
	//	GetCharacterMovement()->AddForce(Correction * 800.f);

	//	// 重力を加える
	//	GetCharacterMovement()->AddForce(FVector(0, 0, -980.f * GetCharacterMovement()->Mass));

	//	// ロープ方向の速度制限を弱める（完全固定だと動けない）
	//	FVector Velocity = GetCharacterMovement()->Velocity;
	//	float SpeedAlongRope = FVector::DotProduct(Velocity, RopeDir);
	//	FVector TangentialVelocity = Velocity - RopeDir * SpeedAlongRope * 0.3; // 0.0～1.0で調整
	//	GetCharacterMovement()->Velocity = TangentialVelocity;
	//}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputConponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputConponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);				//ジャンプ
		EnhancedInputConponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);		//ジャンプ停止
																														
		EnhancedInputConponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);			//移動
																														
		EnhancedInputConponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);			//視点移動
																																																											
		//EnhancedInputConponent->BindAction(PersAction, ETriggerEvent::Started, this, &AMyCharacter::Pers);				//視点切り替え
																														
		//EnhancedInputConponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMyCharacter::Run);				//ダッシュ
		//EnhancedInputConponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMyCharacter::StopRun);

		EnhancedInputConponent->BindAction(GrappleAction, ETriggerEvent::Started, this, &AMyCharacter::Grappling);			//ダッシュ停止
		EnhancedInputConponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &AMyCharacter::Fire);			//ダッシュ停止
		//EnhancedInputConponent->BindAction(GrappleAction, ETriggerEvent::Completed, this, &AMyCharacter::StopGrapple);			//ダッシュ停止


	}
}

void AMyCharacter::KeyAdd()
{
	GameInstance->keycount++;

	if (MainWidgetInstance)
	{
		MainWidgetInstance->SetKeyCount(GameInstance->keycount);
	}
}

void AMyCharacter::KeySub()
{
	GameInstance->keycount--;

	if (MainWidgetInstance)
	{
		MainWidgetInstance->SetKeyCount(GameInstance->keycount);
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && GetCharacterMovement()->IsFalling() == false || bIsFiringGrapple == false && isGrappling == false)
	{
		//キャラ正面を取得
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 正面へのベクトルを取得
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// 横方向のベクトルを取得 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 動きに反映
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	// 2軸のベクトルを取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		float LookSensitivity = 0.5f;
		//マウスの動きにあわせて視点に反映
		AddControllerYawInput(LookAxisVector.X * LookSensitivity);
		AddControllerPitchInput(-LookAxisVector.Y * LookSensitivity);
	}
}

//視点切り替え
void AMyCharacter::Pers(const FInputActionValue& Value)
{
	isPers = !isPers;

	if (isPers)
	{
		ThirdPersonCamera->SetActive(false);
		FirstPersonCamera->SetActive(true);
	}
	else
	{
		ThirdPersonCamera->SetActive(true);
		FirstPersonCamera->SetActive(false);
	}
}

//ダッシュ
void AMyCharacter::Run(const FInputActionValue& Value)
{
	isRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

//ダッシュ停止
void AMyCharacter::StopRun(const FInputActionValue& Value)
{
	isRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

//グラップル
void AMyCharacter::Grappling(const FInputActionValue& Value)
{
	if (bIsFiringGrapple) return;

	if (!isGrappling)
	{
		GrappleCable->SetVisibility(true);
		bIsFiringGrapple = true;
		bHasHitTarget = false;

		GrappleStart = FirstPersonCamera->GetComponentLocation();
		CableStart = GetMesh()->GetSocketLocation(TEXT("RightHand"));
		GrappleDir = FirstPersonCamera->GetForwardVector();

		GrappleTip = GrappleStart; // 先端はまだカメラ位置から
		CurrentCableLength = 0.f;
	}
	else
	{
		isGrappling = false;
		bIsFiringGrapple = false;

		GrappleCable->SetVisibility(false);

		if (GrappleAnchor)
		{
			GrappleAnchor->DestroyComponent();
			GrappleAnchor = nullptr;
		}

		// 通常の移動に戻す
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
}

//グラップル停止
void AMyCharacter::StopGrapple(const FInputActionValue& Value)
{
	if (!isGrappling && !bIsFiringGrapple) return;

	isGrappling = false;
	bIsFiringGrapple = false;

	GrappleCable->SetVisibility(false);

	if (GrappleAnchor)
	{
		GrappleAnchor->DestroyComponent();
		GrappleAnchor = nullptr;
	}

	// 通常の移動に戻す
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

void AMyCharacter::Fire(const FInputActionValue& Value)
{
	

	if (KnifeClass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("in world"));
			// 自分を操作している Controller を取得
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			if (PlayerController == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerController NULL"));
				return;
			}

			// カメラの向き（プレイヤーの場合）
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

			FVector MuzzleOffset = FVector(100.f, 0.f, 80.f); // 必要なら変更
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;


			World->SpawnActor<AThrowKnifeActor>(KnifeClass,SpawnLocation,SpawnRotation,ActorSpawnParams);

		}
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("no class"));
	}
}