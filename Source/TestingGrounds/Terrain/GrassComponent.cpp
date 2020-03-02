// Fill out your copyright notice in the Description page of Project Settings.


#include "GrassComponent.h"
#include "Tile.h"

UGrassComponent::UGrassComponent()
{

}

void UGrassComponent::BeginPlay()
{
    Super::BeginPlay();
    SpawnGrass(SpawnCount);
}

void UGrassComponent::SpawnGrass(int32 SpawnMax)
{
    auto Tile = Cast<ATile>(GetAttachmentRootActor());
    for (size_t i=0;i<SpawnMax;i++)
    {
        auto Location = Tile->GetEmptyLocation(20);
        if(Location!= FVector::ZeroVector)
        {
            AddInstance(FTransform(Location)); 
        }
    }   
    return;
}


