#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "BlindItem.generated.h"

UCLASS()
class PROJECT8_API ABlindItem : public AItemBase
{
	GENERATED_BODY()

public:
	ABlindItem();
	
	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Duration;
};
