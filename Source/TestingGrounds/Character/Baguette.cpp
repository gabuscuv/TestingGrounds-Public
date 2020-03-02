// Fill out your copyright notice in the Description page of Project Settings.


#include "Baguette.h"


// Sets default values
ABaguette::ABaguette()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	RootComponent = FP_Gun;
	FP_Gun->SetOnlyOwnerSee(false);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	//FP_Gun->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABaguette::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaguette::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

