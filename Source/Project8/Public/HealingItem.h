#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "HealingItem.generated.h"

UCLASS()
class PROJECT8_API AHealingItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	AHealingItem();

	virtual void ActivateItem(AActor* Activator) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount;
	
};
