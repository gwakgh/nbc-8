#include "Spike.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASpike::ASpike()
{
	ItemType = "Spike";

	SpikeDamage = 15;
	SpikeRadius = 100;
	VisibleDuration = 3.f;
	HiddenDuration = 1.2f;

	bIsActivating = false;
	bIsDamaged = false;

	Mesh->SetCollisionProfileName(FName("NoCollision"));
	
	DamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	DamageCollision->InitSphereRadius(SpikeRadius);
	DamageCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DamageCollision->SetupAttachment(Root);

	RotatingMovementComp->RotationRate = FRotator::ZeroRotator;

}

void ASpike::BeginPlay()
{
	Super::BeginPlay();

	Disappear();
}
void ASpike::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex)
{
	Super::OnItemEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
	bIsDamaged = false;
}

void ASpike::Disappear()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	bIsActivating = false;
	bIsDamaged = false;

	GetWorld()->GetTimerManager().SetTimer(
		VisibleTimerHandle,
		this,
		&ASpike::Appear,
		HiddenDuration
		);
}

void ASpike::Appear()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	bIsActivating = true;

	TArray<AActor*> OverlappingActors;
	DamageCollision->GetOverlappingActors(OverlappingActors, TSubclassOf<ACharacter>(ACharacter::StaticClass()));

	for (AActor* OverlappingActor : OverlappingActors)
	{
		ActivateItem(OverlappingActor);
	}

	GetWorld()->GetTimerManager().SetTimer(
		VisibleTimerHandle,
		this,
		&ASpike::Disappear,
		VisibleDuration
		);
}

void ASpike::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (!bIsDamaged && bIsActivating)
		{
			bIsDamaged = true;
			
			UGameplayStatics::ApplyDamage(
				Activator,
				SpikeDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}
}

