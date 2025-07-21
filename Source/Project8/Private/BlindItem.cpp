#include "BlindItem.h"
#include "Project8PlayerController.h"

ABlindItem::ABlindItem()
{
	ItemType = "BlindItem";
	Duration = 5.f;
}

void ABlindItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (APawn* Pawn = Cast<APawn>(Activator))
		{
			if (AProject8PlayerController* PC = Cast<AProject8PlayerController>(Pawn->GetController()))
			{
				PC->GetMenuComponent()->ShowBlindMenu(Duration);
			}
		}
		DestroyItem();
	}
}
