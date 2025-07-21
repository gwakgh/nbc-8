#include "ReverseItem.h"
#include "Project8Character.h"
#include "Project8PlayerController.h"

AReverseItem::AReverseItem()
{
	ItemType = "Reverse";
	Duration = 5.f;
}

void AReverseItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AProject8Character* PlayerCharacter = Cast<AProject8Character>(Activator))
		{
			if (AProject8PlayerController* PC = Cast<AProject8PlayerController>(PlayerCharacter->GetController()))
			{
				PC->ActivateReverseMovement(5.0f);
			}
		}
		DestroyItem();
	}
}
