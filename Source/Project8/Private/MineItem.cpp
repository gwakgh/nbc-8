#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 2.5f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
	bIsExploding = false;
    
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Root);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (!bIsExploding)
	{
		bIsExploding = true;
		GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);
		Super::ActivateItem(Activator);
	}
}

void AMineItem::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}

	PlayEffects();
	DestroyItem();
}
void AMineItem::PlayEffects()
{
	if (ExploParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExploParticle, GetActorLocation(), GetActorRotation(), true);
	}

	if (ExploSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExploSound, GetActorLocation());
	}
}
