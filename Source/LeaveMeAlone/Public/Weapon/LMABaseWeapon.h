// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfAmmo);



class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FAmmoWeapon {
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  int32 Bullets;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  int32 Clips;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor {
  GENERATED_BODY()

public:
  ALMABaseWeapon();

  void Fire();
  void ChangeClip();
  FOnOutOfAmmo OnOutOfAmmo;
  bool IsClipFull() const;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
  USkeletalMeshComponent *WeaponComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  float TraceDistance = 800.0f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  FAmmoWeapon AmmoWeapon{30, 0, true};

  virtual void BeginPlay() override;

  void Shoot();
  void DecrementBullets();
  bool IsCurrentClipEmpty() const;
  

public:
  virtual void Tick(float DeltaTime) override;

private:
  FAmmoWeapon CurrentAmmoWeapon;
};