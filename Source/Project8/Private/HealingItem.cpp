#include "HealingItem.h"
#include "Project8Character.h"

AHealingItem::AHealingItem()
{
	ItemType = "Healing";
	HealAmount = 20.f;
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AProject8Character* PlayerCharacter = Cast<AProject8Character>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
			DestroyItem();
	}
}
