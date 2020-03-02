// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "ActorComponents/ActorPool.h"
#include "CompatibilityLayer/Macros.h"
#include "ActorComponents/ActorGarbageCollector.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SharedRoot = CreateDefaultSubobject<USceneComponent>(FName("SharedRoot"));
	SetRootComponent(SharedRoot);
	
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(FName("Floor 2"));
	Floor->SetupAttachment(SharedRoot);
	Floor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Floor->SetCollisionProfileName(FName("BlockAll"));
//	Floor->SetRelativeTransform(FTransform(FVector(0,0,-50)));

	NavigationBoundsOffset = FVector(0, 4000, 0);
	MaxExtent = FVector( 2000, 2000, Floor->GetRelativeTransform().GetLocation().Z);
	MinExtent = FVector(-2000,-2000, Floor->GetRelativeTransform().GetLocation().Z);
	BoxSpawn = FBox(MinExtent,MaxExtent);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
//	BoxSpawn = Floor->CalcBounds(SharedRoot->GetComponentTransform()).GetBox();

//	CastSphere(GetActorLocation(),300);
}

void ATile::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if(Pool!=nullptr)
	{
		Pool->Return(NavMesh);
	}
//	UE_LOG(LogTemp,Warning,TEXT("Died"));
	Super::EndPlay(EndPlayReason);

//	CastSphere(GetActorLocation(),300);
}

void ATile::SetPool(UActorPool* InPool)
{
	if (InPool==nullptr){UE_LOG(LogTemp,Error,TEXT("[%f][%s][%s]: InPool Null"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));;return;}
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	
	NavMesh = Pool->Checkout();
	if (NavMesh==nullptr){UE_LOG(LogTemp,Error,TEXT("[%f][%s][%s]: NavMesh Null"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));;return;}
//	UE_LOG(LogTemp,Error,TEXT("[%f][%s][%s]: Before NavMesh @ %s"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__),*NavMesh->GetActorLocation().ToCompactString());
	NavMesh->SetActorLocationAndRotation(GetActorLocation()+NavigationBoundsOffset,
										NavMesh->GetActorRotation(),false,nullptr,ETeleportType::ResetPhysics);
	
//	UE_LOG(LogTemp,Error,TEXT("[%f][%s][%s]: After NavMesh @ %s"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__),*NavMesh->GetActorLocation().ToCompactString());
	FNavigationSystem::Build(*GetWorld());
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	//SweepSingleByChannel( FHitResult& OutHit,  FVector& Start, FVector& End, FQuat& Rot, ECollisionChannel TraceChannel, FCollisionShape& CollisionShape, FCollisionQueryParams& Params /* = FCollisionQueryParams::DefaultQueryParam */, const FCollisionResponseParams& ResponseParam ) 
	FHitResult Hit;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool hasHit= GetWorld()->SweepSingleByChannel(Hit,
									GlobalLocation,
									GlobalLocation,
									FQuat::Identity,
									ECollisionChannel::ECC_GameTraceChannel2,
									FCollisionShape::MakeSphere(Radius));
	
	return hasHit;
}

FVector ATile::GetEmptyLocation(float Radius)
{	
	FVector Location;
	int32 cont=0;
	do
	{
		Location = FMath::RandPointInBox(BoxSpawn);
		cont++;
	}while(CanSpawnAtLocation(Location,Radius) && cont!=AttemptActorAllocation);
	return Location;
}

TArray<FSpawnPosition> ATile::SpawnPositionGenerator(int32 NumSpawn, float radius, int32 minRotation, int32 maxRotation, 
	int32 minScale, int32 maxScale)
{
	TArray<FSpawnPosition> spawns;
	for(size_t i = 0;i < NumSpawn;i++)
	{
		FSpawnPosition SpawnPositon = FSpawnPosition(
			
			minRotation,maxRotation,
			minScale,maxScale);
		SpawnPositon.Location = GetEmptyLocation(radius * SpawnPositon.Scale.X * SpawnPositon.Scale.Y);
		spawns.Add(SpawnPositon);
	}
	return spawns;
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	auto spawns = SpawnPositionGenerator(FMath::RandRange(MinSpawn,MaxSpawn),Radius,-180,180);
	for(auto spawn : spawns)
	{
			PlaceActor(ToSpawn,spawn);							
	}
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius)
{
	RandomlyPlaceActors(ToSpawn,MinSpawn,MaxSpawn,Radius);
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	RandomlyPlaceActors(ToSpawn,MinSpawn,MaxSpawn,Radius,MinScale,MaxScale);
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	if(SpawnPosition.Location != FVector::ZeroVector && ! SpawnPosition.Rotation.ContainsNaN())
	{

		auto Actor =GetWorld()->SpawnActor<APawn>(ToSpawn,
													FTransform(
																SpawnPosition.Rotation,
																SpawnPosition.Location + FVector(0,0,100),
																FVector(SpawnPosition.Scale)
															)
													,FActorSpawnParameters());
		if(Actor == nullptr){UE_LOG(LogTemp,Warning,TEXT("[%f][%s][%s]:Pawn Allocation failed"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));return;}
		Actor->Tags.Push(FName("Enemy"));
		Actor->AttachToActor(this,FAttachmentTransformRules(EAttachmentRule::KeepRelative,false));
		Actor->SpawnDefaultController();
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("[%f][%s][%s]Pawn Allocation failed"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));return;
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	if(SpawnPosition.Location!=FVector::ZeroVector)
	{
		auto Actor = GetWorld()->SpawnActor<AActor>(ToSpawn,
													FTransform(
																SpawnPosition.Rotation,
																SpawnPosition.Location,
																FVector(SpawnPosition.Scale)
															)
													,FActorSpawnParameters());
		if(Actor==nullptr){	UE_LOG(LogTemp,Error,TEXT("[%f][%s][%s]: Actor Allocation failed"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));;return;}
		Actor->AttachToActor(this,FAttachmentTransformRules(EAttachmentRule::KeepRelative,false));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%f][%s][%s]: Actor Allocation failed"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));return;
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

