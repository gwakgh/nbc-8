#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Sphere->SetSphereRadius(100.0f);
	Sphere->SetupAttachment(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnItemOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnItemEndOverlap);
	
	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
	RotatingMovementComp->RotationRate = FRotator(0.0f, 90.0f, 0.0f);
}

void AItemBase::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ActivateItem(OtherActor);
	}
}

void AItemBase::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void AItemBase::ActivateItem(AActor* Activator)
{
	UWorld* World = GetWorld();
	if (World && PickupParticle)
	{
		UParticleSystemComponent* SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(
			World,
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			false);
		
		if (SpawnedParticle)
		{
			FTimerHandle ParticleTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				ParticleTimerHandle,
				[SpawnedParticle]()
				{
					if (SpawnedParticle && SpawnedParticle->IsValidLowLevelFast())
					{
						SpawnedParticle->DestroyComponent();
					}
				},
				0.9f,
				false
			);
		}
	}

	if (World && PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, PickupSound, GetActorLocation());
	}
}

FName AItemBase::GetItemType() const
{
	return ItemType;
}

void AItemBase::DestroyItem()
{
	Destroy();
}
