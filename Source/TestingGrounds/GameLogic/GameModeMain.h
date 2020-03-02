// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeMain.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API AGameModeMain : public AGameModeBase
{
	GENERATED_BODY()
	
	
	AGameModeMain();

	protected:
	UPROPERTY(BlueprintReadWrite, Category="Nav Mesh Bounds Volume Pool")
	class UActorPool* NavMeshBoundsVolume;
	UPROPERTY(BlueprintReadWrite, Category="NextTileTransform")
	FTransform NextTileTransform;
	UPROPERTY(EditDefaultsOnly, Category="NextTileTransform")
	FVector SafeTeleportOffset;

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category="Stuff")
	void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);
	UFUNCTION(BlueprintCallable, Category="Stuff")
	void PopulateBoundsVolumePool();
	UFUNCTION(BlueprintCallable, Category="Stuff")
	int32 TileCount() const;

	public:
	UFUNCTION(BlueprintCallable, Category="Stuff")
	bool TeleportToSafeLocation(AActor* ActortoSafe);

	private:
	FVector GetSafeLocation();
};
