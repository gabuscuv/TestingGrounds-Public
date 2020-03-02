// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
	UGrassComponent();

	public:
	UPROPERTY(EditAnywhere,Category = "Spawn")
	int32 SpawnCount;
	UFUNCTION(BlueprintCallable, Category = "Grass")
	void SpawnGrass(int32 SpawnMax);
	private:
	virtual void BeginPlay() override;
	
};
