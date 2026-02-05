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

	//  カメラアーム(pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// 三人称カメラ
	//ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	//ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//ThirdPersonCamera->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// 一人称カメラ
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(CameraBoom); //     Socket ɂ       z  

	CableStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CableStartPoint"));
	CableStartPoint->SetupAttachment(FirstPersonCamera);

	// ケーブルが出る位置
	CableStartPoint->SetRelativeLocation(FVector(30.f, 20.f, -10.f));


	//ロープのケーブル作成
	GrappleCable = CreateDefaultSubobject<UCableComponent>(TEXT("GrappleCable"));
	GrappleCable->SetupAttachment(CableStartPoint);
	GrappleCable->SetVisibility(false);
	
	//GrappleCable->bEnableStiffness = true;	//   ͂ L  
	GrappleCable->bEnableCollision = true;		// Փ˔    ؂ 
	GrappleCable->NumSegments = 10;
	GrappleCable->SolverIterations = 16;
	//
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

	if (BGM != nullptr)
		UGameplayStatics::PlaySoundAtLocation(this, BGM, GetActorLocation());

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

//   t   [   X V
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//  P [ u     Ē  i L ΂    j
		if (bIsFiringGrapple)
		{
			//   [  L ΂ 
			float FireSpeed = 4000.f;
			CurrentCableLength += FireSpeed * DeltaTime;

			//FVector AdjustedDir = GrappleDir;
			//AdjustedDir = FVector(0.f,0.f,0.f); // X      [   ɂ  Ė   
			//AdjustedDir.Normalize(); //    K         i d v j

			//      X  +    Ă  ܂  ̂      ==  ~
			//GrappleTip = GrappleStart + GrappleDir * CurrentCableLength;

			//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), GrappleTip.X, GrappleTip.Y, GrappleTip.Z);
			//    C  s x  ΂  i  [ ܂Łj
			// O   b v   V  p   C
			bool gHit;
			FHitResult GraHit;
			FCollisionQueryParams GrappleParams;
			GrappleParams.AddIgnoredActor(this);
			FCollisionObjectQueryParams  GraObjParams;
			GraObjParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
			// M ~ b N p   C
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

			//  L ΂  \  ̐ [ ʒu
			if (!bHit && !gHit)
			{

				FVector DesiredTip = GrappleStart + GrappleDir * CurrentCableLength;

				//   Ƀu   b J [    i ǂȂǁj
				FHitResult BlockHit;
				bool bBlocked = GetWorld()->LineTraceSingleByChannel(
					BlockHit,
					GrappleStart,
					DesiredTip,
					ECC_Visibility //  ǁE     Block    TraceChannel
				);

				//  u   b N    Ă     A     Ŏ~ ߂ 
				if (bBlocked)
				{
					GrappleTip = BlockHit.Location; //  ǂ܂ 
					// O   b v    ~
					bIsFiringGrapple = false;
					GrappleCable->SetVisibility(false);
					if (GrappleAnchor)
					{
						GrappleAnchor->DestroyComponent();
						GrappleAnchor = nullptr;
					}

					gHit = false;            //  V  Ƀq b g   Ă  Ȃ       
					bHit = false;            //  V  Ƀq b g   Ă  Ȃ       
					return;                         //     ŏ    I     d v I
				}
			}

			if (bHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("HitActor: %s"), *GimHit.GetActor()->GetName());
			}

			//    o m F p   C  
			//DrawDebugLine(GetWorld(), GrappleStart, GrappleTip, FColor::Green, false, -1.0f, 0, 2.0f);

			//  P [ u   ̐ [ ʒu X V
			if (!gHit)
				GrappleCable->SetWorldLocation(GrappleStart);


			if (!bHasHitTarget)
			{

				if (GrappleCable)
				{
					// CableComponent   ̂̈ʒu ́u J n _( \ P b g) v ̂܂܂ɂ  Ă    A I [ A ^ b `  iAnchor j 𓮂         ʓI
					// Anchor         ΍쐬     FirstPersonCamera  ɑ  ΃A ^ b `   Ă   
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
						// Anchor    [ ʒu Ɉړ      A P [ u   ̏I _   Anchor  ɃA ^ b `
						GrappleAnchor->SetWorldLocation(GrappleTip);
						GrappleCable->SetAttachEndToComponent(GrappleAnchor, NAME_None);
					}
				}

			}

			//          u  
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
						if (GimmickClass->ButtonSound != nullptr)
						{
							UGameplayStatics::PlaySoundAtLocation(this, GimmickClass->ButtonSound, GetActorLocation());
						}
					}
					else if (AAttractFloorActor* AttractClass = Cast<AAttractFloorActor>(HitActor))
					{
						if (AttractClass->AttractSound != nullptr)
						{
							UGameplayStatics::PlaySoundAtLocation(this, AttractClass->AttractSound, GetActorLocation());
						}
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

				gHit = false;            //  V  Ƀq b g   Ă  Ȃ       
				bHit = false;            //  V  Ƀq b g   Ă  Ȃ       
				return;                  //        ŏ    I     d v I

			}
			if (gHit && !bHasHitTarget)
			{
				GrappleCable->SetWorldLocation(CableStart);

				bHasHitTarget = true;
				bIsFiringGrapple = false;
				isGrappling = true;

				//  q b g     R   | [ l   g
				USceneComponent* HitComp = GraHit.GetComponent();

				if (HitComp && GrappleAnchor)
				{
					//        񃏁[   h ʒu    킹  
					GrappleAnchor->SetWorldLocation(GraHit.ImpactPoint);

					//          j S F V  ɃA ^ b `
					GrappleAnchor->AttachToComponent(
						HitComp,
						FAttachmentTransformRules::KeepWorldTransform
					);
				}

				//        [ v  
				CurrentCableLength = FVector::Distance(
					GetActorLocation(),
					GrappleAnchor->GetComponentLocation()
				);

				TargetCableLength = CurrentCableLength;
				//UE_LOG(LogTemp, Warning, TEXT("Grapple Hit: %s"), *GrabPoint.ToString());
			}

			//   苗   ܂ŐL ΂    ̂ɓ     Ȃ      烊 Z b g
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


		//  U  q        
		if (isGrappling)
		{
			CableStart = GetMesh()->GetSocketLocation(TEXT("RightHand"));
			GrappleCable->SetWorldLocation(CableStart);

			const float MinCableLength = 300.f;

			//      ܂ł  Max  ŋ    I ɏk ߂Ă      A
			//            炩 ȕ Ԃɕς   
			float InterpSpeed = 500.f; //               Ȃ 20?30 ɂ   OK

			CurrentCableLength = FMath::FInterpTo(
				CurrentCableLength,   //    ݂̒   
				MinCableLength,       //  ڕW ̒    i       j
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


	//  U  q             
	//if (isGrappling)
	//{
	//	FVector ActorLoc = GetActorLocation();
	//	FVector ToAnchor = GrabPoint - ActorLoc;
	//	float dDistanceToAnchor = ToAnchor.Size();
	//	FVector RopeDir = ToAnchor.GetSafeNormal();

	//	//    [ v ̒      Ɍ  ݂̋    ɒǏ]      
	//	CurrentCableLength = DistanceToAnchor;

	//	//    [ v ̒  ͕␳ i y       ߂  悤 Ɂj
	//	FVector CorrectedPos = GrabPoint - RopeDir * CurrentCableLength;
	//	FVector Correction = CorrectedPos - ActorLoc;
	//	GetCharacterMovement()->AddForce(Correction * 800.f);

	//	//  d ͂      
	//	GetCharacterMovement()->AddForce(FVector(0, 0, -980.f * GetCharacterMovement()->Mass));

	//	//    [ v     ̑  x       ߂ i   S Œ肾 Ɠ    Ȃ  j
	//	FVector Velocity = GetCharacterMovement()->Velocity;
	//	float SpeedAlongRope = FVector::DotProduct(Velocity, RopeDir);
	//	FVector TangentialVelocity = Velocity - RopeDir * SpeedAlongRope * 0.3; // 0.0 `1.0 Œ   
	//	GetCharacterMovement()->Velocity = TangentialVelocity;
	//}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputConponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputConponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);					// W     v
		EnhancedInputConponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);			// W     v  ~
																														
		EnhancedInputConponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);				// ړ 
																														
		EnhancedInputConponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);				//   _ ړ 
																																																											
		//EnhancedInputConponent->BindAction(PersAction, ETriggerEvent::Started, this, &AMyCharacter::Pers);				//   _ ؂ ւ 
																														
		//EnhancedInputConponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMyCharacter::Run);				// _ b V  
		//EnhancedInputConponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMyCharacter::StopRun);			// _ b V    ~

		EnhancedInputConponent->BindAction(GrappleAction, ETriggerEvent::Started, this, &AMyCharacter::Grappling);			//   [ v    
		EnhancedInputConponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &AMyCharacter::Fire);					// i C t    
		//EnhancedInputConponent->BindAction(GrappleAction, ETriggerEvent::Completed, this, &AMyCharacter::StopGrapple);	//   [ v  ~


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
		// L       ʂ 擾
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//    ʂւ̃x N g    擾
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//        ̃x N g    擾 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//      ɔ  f
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	// 2   ̃x N g    擾
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		float LookSensitivity = 0.5f;
		// } E X ̓    ɂ  킹 Ď  _ ɔ  f
		AddControllerYawInput(LookAxisVector.X * LookSensitivity);
		AddControllerPitchInput(-LookAxisVector.Y * LookSensitivity);
	}
}

//   _ ؂ ւ 
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

// _ b V  
void AMyCharacter::Run(const FInputActionValue& Value)
{
	isRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

// _ b V    ~
void AMyCharacter::StopRun(const FInputActionValue& Value)
{
	isRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

// O   b v  
void AMyCharacter::Grappling(const FInputActionValue& Value)
{
	if (bIsFiringGrapple) return;

	if (!isGrappling)
	{
		if (RopeSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, RopeSound, GetActorLocation());
		}

		GrappleCable->SetVisibility(true);
		bIsFiringGrapple = true;
		bHasHitTarget = false;

		GrappleStart = FirstPersonCamera->GetComponentLocation();
		CableStart = GetMesh()->GetSocketLocation(TEXT("RightHand"));
		GrappleDir = FirstPersonCamera->GetForwardVector();

		GrappleTip = GrappleStart; //   [ ͂܂  J     ʒu    
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

		//  ʏ ̈ړ  ɖ߂ 
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
}

// O   b v    ~
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

	//  ʏ ̈ړ  ɖ߂ 
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

void AMyCharacter::Fire(const FInputActionValue& Value)
{
	if (KnifeClass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (ThrowSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, ThrowSound, GetActorLocation());

		if (World != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("in world"));
			//      𑀍삵 Ă    Controller   擾
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			if (PlayerController == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerController NULL"));
				return;
			}

			//  J     ̌    i v   C   [ ̏ꍇ j
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

			FVector MuzzleOffset = FVector(100.f, 0.f, 80.f); //  K v Ȃ ύX
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