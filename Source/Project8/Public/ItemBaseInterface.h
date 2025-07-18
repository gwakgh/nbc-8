#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemBaseInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemBaseInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT8_API IItemBaseInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	UFUNCTION()
	
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	UFUNCTION()
	virtual void ActivateItem(AActor* Activator) = 0;
	
	UFUNCTION()
	virtual FName GetItemType() const = 0;
};
