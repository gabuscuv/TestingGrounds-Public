// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeMain.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorComponents/ActorPool.h"
#include "Terrain/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Character/TestingGroundsHUD.h"
#include "UObject/ConstructorHelpers.h"

AGameModeMain::AGameModeMain(): Super()
{
		// set default pawn class to our Blueprinted character
//	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Player/FirstPersonCharacter"));
//	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGameModeMain::StaticClass();
	NavMeshBoundsVolume = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volume Pool"));
	SafeTeleportOffset = FVector(0,-4000,400);
}

void AGameModeMain::BeginPlay()
{
    Super::BeginPlay();
//    PopulateBoundsVolumePool();
}

void AGameModeMain::AddToPool(ANavMeshBoundsVolume* VolumeToAdd)
{
	NavMeshBoundsVolume->Add(VolumeToAdd);
}

void AGameModeMain::PopulateBoundsVolumePool()
{
	if(GetWorld()==nullptr){return;}
	if(NavMeshBoundsVolume->Num() > 0){return;}
	auto ActorIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

	while(ActorIterator)
	{
//		UE_LOG(LogTemp, Display, TEXT("[%f][%s][%s]Adding VolPools to Array: %s"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__),*ActorIterator->GetName());
		AddToPool(*ActorIterator);
		++ActorIterator;
	}
}

bool AGameModeMain::TeleportToSafeLocation(AActor* ActortoSafe)
{
	return	ActortoSafe->SetActorLocationAndRotation(GetSafeLocation(), ActortoSafe->GetActorRotation());
}

FVector AGameModeMain::GetSafeLocation()
{
	return ( NavMeshBoundsVolume->GetSafeLocation().GetLocation() + SafeTeleportOffset);
}

int32 AGameModeMain::TileCount() const
{
	 TArray < AActor * > Tiles;
	 UGameplayStatics::GetAllActorsOfClass(GetWorld(),ATile::StaticClass(),Tiles);
	return Tiles.Num();
}

