// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"
#include "Camera/CameraComponent.h"
#include "Gun.h"
#include "Baguette.h"
#include "Kismet/GameplayStatics.h" 
#include "CompatibilityLayer/Macros.h"
#include "GameLogic/GameModeMain.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// FPCamera Location= X=-34.5599976 Y=21.75 Z=39 
	FPCamera = CreateDefaultSubobject<UCameraComponent>("FPCamera");
	FPCamera->SetupAttachment(RootComponent);
	FPCamera->SetRelativeLocation(FVector(-39.56f,1.75f,64.f));
	// FPArms Location= X=-10.5600014 Y=-12.75 Z=-156.0 
	FPArms = CreateDefaultSubobject<USkeletalMeshComponent>("FPArms");
	FPArms->SetOnlyOwnerSee(true);
	FPArms->SetupAttachment(FPCamera);
	NormalMode();
	// CameraBoom Z=30
	// TPCamera   Z=10
	/*
	GunActor = CreateDefaultSubobject<UChildActorComponent>("GunActor");
	GunActor->SetupAttachment(FPArms);
	*/
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(true);
	
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);

	Baguette = GetWorld()->SpawnActor<ABaguette>(MeleeBlueprint);

	RestoreController();

	Gun->AnimInstance1P = FPArms->GetAnimInstance();
	Gun->AnimInstance3P = GetMesh()->GetAnimInstance();
}

void AMannequin::Melee()
{
	return;
}

void AMannequin::RestoreController()
{
	if(IsPlayerControlled())
	{
		Gun->AttachToComponent(FPArms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true), TEXT("GripPoint"));
	}
	else
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true), TEXT("GripPoint"));
	}
	if(InputComponent==nullptr){return;}
	InputComponent->BindAction("Fire",IE_Pressed,this,&AMannequin::PullTrigger);
	InputComponent->BindAction("Aim",IE_Pressed,this,&AMannequin::AimMode);
	InputComponent->BindAction("Aim",IE_Released,this,&AMannequin::NormalMode);
	InputComponent->BindAction("Melee",IE_Pressed,this,&AMannequin::Melee);

}

void AMannequin::NormalMode()
{
	// Maybe play some animation magic
	FPArms->SetRelativeLocationAndRotation(InitialParams.Position,InitialParams.Rotation);
}

void AMannequin::AimMode()
{
	// Maybe play some animation magic
	FPArms->SetRelativeLocationAndRotation(AimParams.Position, AimParams.Rotation);
}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	if(!IsPlayerControlled())
	{
		SetActorEnableCollision(false);
	}
	if(Gun==nullptr){return;}
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true), TEXT("GripPoint"));
}

void AMannequin::PullTrigger()
{
	if(Gun==nullptr){UE_LOG(LogTemp,Error,TEXT("[%s][%s]: Gun is nullptr"),*GetName(),*FString(__PRETTY_FUNCTION__));return;}
	Gun->OnFire();
}

void AMannequin::AddAmmo(int32 Ammo)
{
	if(Gun==nullptr){UE_LOG(LogTemp,Error,TEXT("[%s][%s]: Gun is nullptr"),*GetName(),*FString(__PRETTY_FUNCTION__));return;}
	Gun->AddAmmo(Ammo);
}

int AMannequin::GetAmmo() const
{
	if(Gun==nullptr){UE_LOG(LogTemp,Error,TEXT("[%s][%s]: Gun is nullptr"),*GetName(),*FString(__PRETTY_FUNCTION__));return 0;}
	return Gun->GetAmmo();
}

int AMannequin::GetMaxAmmo() const
{
	if(Gun==nullptr){UE_LOG(LogTemp,Error,TEXT("[%s][%s]: Gun is nullptr"),*GetName(),*FString(__PRETTY_FUNCTION__));return 0;}
	return Gun->GetMaxAmmo();
}



// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AMannequin::Fire);
}

void AMannequin::FellOutOfWorld(const UDamageType& dmgType)
{
	if(this->IsPlayerControlled())
	{
		if(UGameplayStatics::GetCurrentLevelName(GetWorld(),true) == "MainLevel")
		{
			Cast<AGameModeMain>(UGameplayStatics::GetGameMode(GetWorld()))->TeleportToSafeLocation(this);
		}else
		{
			Super::FellOutOfWorld(dmgType);
		//	UGameplayStatics::GetGameState(GetWorld())->
		}
	}
}