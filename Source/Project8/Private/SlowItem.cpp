#include "SlowItem.h"
#include "Project8Character.h"

ASlowItem::ASlowItem()
{
	ItemType = "SpeedItem";
	Amount = 20.f;
	Duration = 5.f;
}

void ASlowItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AProject8Character* PlayerCharacter = Cast<AProject8Character>(Activator))
		{
			Amount = FMath::RandRange(-300.f, 300.f);
			PlayerCharacter->ApplySlowEffect(Amount, Duration);
		}
		DestroyItem();
	}
}
