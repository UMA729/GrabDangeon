// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowKnifeActor.h"
#include "EnemyCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AThrowKnifeActor::AThrowKnifeActor()
{

	KunaiCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShereComp"));
	KunaiCollision->InitSphereRadius(10.0f);
	KunaiCollision->BodyInstance.SetCollisionProfileName("Projectile");

	KunaiCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	KunaiCollision->SetCollisionObjectType(ECC_WorldDynamic);

	// Hit ‚ðŽg‚¤‚½‚ß Pawn ‚ð Block ‚É‚·‚éiOverlap ‚Å‚Í Hit ‚Ío‚È‚¢j
	KunaiCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	KunaiCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	KunaiCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	KunaiCollision->SetNotifyRigidBodyCollision(true); // OnHit ‚É•K{
	KunaiCollision->SetGenerateOverlapEvents(false);   // Overlap ‚ÍŽg‚í‚È‚¢

	// ‚·‚è”²‚¯–hŽ~ (‚‘¬Projectile‚É•K{)
	KunaiCollision->BodyInstance.bUseCCD = true;


	RootComponent = KunaiCollision;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = KunaiCollision;
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	KunaiCollision->OnComponentHit.AddDynamic(this, &AThrowKnifeActor::OnHit);		// set up a notification for when this component hits something blocking

	knife_damage = 50.f;

	InitialLifeSpan = 3.0f;
}

void AThrowKnifeActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"),		*GetNameSafe(OtherActor));

	if (AEnemyCharacter* EnemyClass = Cast<AEnemyCharacter>(OtherActor))
	{
		EnemyClass->HP -= knife_damage;

		UE_LOG(LogTemp, Warning, TEXT("EnemyHit"));

		Destroy();
	}
}