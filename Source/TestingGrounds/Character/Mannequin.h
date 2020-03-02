// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"

USTRUCT()
struct FGunPosition
{
		GENERATED_USTRUCT_BODY()
		FGunPosition(){}
		FGunPosition(FVector position,FRotator rotation)
		{
			this->Position= position;
			this->Rotation= rotation;
		}
	FVector Position;
	FRotator Rotation;
};


UCLASS()
class TESTINGGROUNDS_API AMannequin : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMannequin();
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TSubclassOf<class ABaguette> MeleeBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TSubclassOf<class AGun> GunBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	FGunPosition InitialParams =FGunPosition(FVector(1,1,-160),FRotator(4.537484,-15.770861,3.173508));
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	FGunPosition AimParams =FGunPosition(FVector(-10.923786,-14.249467,-160),FRotator(4.537484,-15.770861,3.173508));
	UPROPERTY(BlueprintReadWrite, Category = Mesh)
	class UCameraComponent* FPCamera;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullTrigger();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Melee();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddAmmo(int32 Ammo = 10);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetMaxAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RestoreController();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
		
	void AimMode();
	void NormalMode();

private:
/*	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UCameraComponent* TPCamera;
	*/


	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class AGun* Gun;
	class ABaguette* Baguette;

protected:
	UPROPERTY(BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* FPArms;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void UnPossessed() override;

};
