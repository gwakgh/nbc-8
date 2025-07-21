#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ReverseItem.generated.h"

UCLASS()
class PROJECT8_API AReverseItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	AReverseItem();
	
	virtual void ActivateItem(AActor* Activator) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Duration;
	
};
