#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "MineItem.generated.h"

class USphereComponent;

UCLASS()
class PROJECT8_API AMineItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	AMineItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExplosionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	int ExplosionDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	UParticleSystem* ExploParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	USoundBase* ExploSound;
    
	FTimerHandle ExplosionTimerHandle;

	bool bIsExploding;

	virtual void ActivateItem(AActor* Activator) override;

	void Explode();
};
