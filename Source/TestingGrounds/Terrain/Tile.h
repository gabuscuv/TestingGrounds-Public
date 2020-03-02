// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"
USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FSpawnPosition(){}

	FSpawnPosition(float minRotation, float maxRotation, float minScale,float maxScale)
	{
		this->Rotation	= FRotator(0,FMath::RandRange(minRotation,maxRotation),0);
		this->Scale		= FVector(FMath::RandRange(minScale,maxScale));
	}

	FSpawnPosition(FVector location, float minRotation, float maxRotation, float minScale,float maxScale)
	{
		this->Location	= location;
		this->Rotation	= FRotator(0,FMath::RandRange(minRotation,maxRotation),0);
		this->Scale		= FVector(FMath::RandRange(minScale,maxScale));
	}

	FSpawnPosition(FVector location, FRotator rotation, FVector scale)
	{
		this->Location	= location;
		this->Rotation	= rotation;
		this->Scale		= scale;
	}

	FVector Location;
	FRotator Rotation;
	FVector Scale;
};

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	ATile();

private:
	class UActorPool* Pool;

	FBox BoxSpawn;

	AActor* NavMesh;

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1,float Radius = 500, float MinScale = 1.0f, float MaxScale = 1.0f);

	void PlaceActor(TSubclassOf<APawn> ToSpawn,FSpawnPosition SpawnPosition);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition Spawnposition);
	
	const int32 AttemptActorAllocation = 10;
	
	void PositionNavMeshBoundsVolume();

public:	
	// Sets default values for this actor's properties

	TArray<FSpawnPosition> SpawnPositionGenerator(int32 NumSpawn,float radius, int32 minRotation = -180,
 	int32 maxRotation = 180, int32 minScale = 1, int32 maxScale = 1);

	FVector GetEmptyLocation(float Radius);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1,float Radius = 500, float MinScale = 1.0f, float MaxScale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn,int32 MinSpawn,int32 MaxSpawn,float Radius = 500);
	
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	bool CanSpawnAtLocation(FVector Location, float Radius = 300);
	
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(class UActorPool* Pool);



protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	USceneComponent* SharedRoot;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Floor;
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnZone")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnZone")
	FVector MaxExtent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
