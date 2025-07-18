#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "CoinItem.generated.h"

UCLASS()
class PROJECT8_API ACoinItem : public AItemBase
{
	GENERATED_BODY()

public:
	ACoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
	
};
