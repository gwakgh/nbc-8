#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Spike.generated.h"

class USphereComponent;

UCLASS()
class PROJECT8_API ASpike : public AItemBase
{
	GENERATED_BODY()
	
public:
	ASpike();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* DamageCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float SpikeDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float SpikeRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	int SpikeDamage;

	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

	void Disappear();
	void Appear();
	FTimerHandle VisibleTimerHandle;

	bool bIsActivating;
	bool bIsDamaged;

	float VisibleDuration;
	float HiddenDuration;

	virtual void ActivateItem(AActor* Activator) override;
	virtual void BeginPlay() override;
};
