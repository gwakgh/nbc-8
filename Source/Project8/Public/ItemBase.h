#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBaseInterface.h"
#include "ItemBase.generated.h"

class USphereComponent;
class URotatingMovementComponent;

UCLASS()
class PROJECT8_API AItemBase : public AActor, public IItemBaseInterface
{
	GENERATED_BODY()
	
public:	
	AItemBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	URotatingMovementComponent* RotatingMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	UParticleSystem* PickupParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	USoundBase* PickupSound;

	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

	virtual void ActivateItem(AActor* Activator) override;

	virtual FName GetItemType() const override;

	virtual void DestroyItem();

};
