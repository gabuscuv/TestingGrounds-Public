// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class TESTINGGROUNDS_API AGun : public AActor
{
	GENERATED_BODY()
		/** Pawn mesh: 1st person view (arms; seen only by self) */
/*	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;
*/
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;
//	class UCameraComponent* FirstPersonCameraComponent;

public:	
	// Sets default values for this actor's properties
	AGun();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int32 AmmoStart = 10;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int32 MaxAmmo = 30;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;
		/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ATestingGroundsProjectile> ProjectileClass;
	void AddAmmo(int32 Ammo);
	FORCEINLINE int GetAmmo() const {return AmmoCurrent;};
	FORCEINLINE int GetMaxAmmo() const {return MaxAmmo;};
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation1P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation3P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimInstance* AnimInstance1P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimInstance* AnimInstance3P;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Fire();
	int32 AmmoCurrent;

public:	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void OnFire();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetFPGun() const { return FP_Gun; }
	
};
