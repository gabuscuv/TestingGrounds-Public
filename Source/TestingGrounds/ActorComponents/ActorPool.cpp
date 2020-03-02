// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"
#include "CompatibilityLayer/Macros.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AActor* UActorPool::Checkout()
{
//	UE_LOG(LogTemp, Warning, TEXT("[%f][%s][%s]: Current Number of Array %d"),GetWorld()->TimeSeconds, *GetName(), *FString(__PRETTY_FUNCTION__), Pool.Num());
	if(Pool.Num() != 0)
	{
		return Pool.Pop();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%f][%s][%s]: Pool Empty"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));
		return nullptr;
	}
}

void UActorPool::Return(AActor* ActorToReturn)
{
	if(ActorToReturn==nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%f][%s][%s]: Tried Remove null Actor"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));
		return;
	}
//	UE_LOG(LogTemp, Display, TEXT("[%f][%s][%s]: Removed %s"),GetWorld()->TimeSeconds, *GetName(),*FString(__PRETTY_FUNCTION__),*ActorToReturn->GetName());
	Add(ActorToReturn);
}

void UActorPool::Add(AActor* ActorToAdd)
{
		
	if(ActorToAdd==nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%f][%s][%s]: Tried add null Actor"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));
		return;
	}
	if(Pool.Contains(ActorToAdd))
	{
	//	UE_LOG(LogTemp, Warning, TEXT("[%f][%s][%s]: Tried add duplicated Actor"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__));
		return;
	}
//	UE_LOG(LogTemp, Display,TEXT("[%f][%s][%s]: %s Added in ActorPool"),GetWorld()->TimeSeconds,*GetName(),*FString(__PRETTY_FUNCTION__),*ActorToAdd->GetName());
	Pool.Push(ActorToAdd);
}

int32 UActorPool::Num() const
{
	return Pool.Num();
}

// Called every frame
void UActorPool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FTransform UActorPool::GetSafeLocation()
{
	FTransform safeLocation = Pool.Top()->GetTransform();

	for(auto actor : Pool)
	{
		if(actor->GetTransform().GetTranslation().Y < safeLocation.GetTranslation().Y)
		{
			safeLocation= actor->GetTransform();
		}
	};
	return safeLocation;
	//return Pool[Pool.Num()-1]->GetTransform();
}