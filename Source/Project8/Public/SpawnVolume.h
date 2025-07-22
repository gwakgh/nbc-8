#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT8_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();
	
	UFUNCTION(BlueprintCallable, Category = "Spawn Volume")
	AActor* SpawnRandomItem();

	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Volume")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Volume")
	UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Volume")
	UDataTable* ItemDataTable;
	
	FVector GetRandomPointInVolume() const;
	
	FItemSpawnRow* GetRandomItem() const;

};
