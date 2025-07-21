#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "SlowItem.generated.h"

UCLASS()
class PROJECT8_API ASlowItem : public AItemBase
{
	GENERATED_BODY()

public:
	ASlowItem();
	
	virtual void ActivateItem(AActor* Activator) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Duration;
	
};
