#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent {
  GENERATED_BODY()

public:
  ULMAWeaponComponent();

  void Fire();
  void Reload();
  void StartFire();
  void StopFire();
  FTimerHandle FireTimerHandle;

  UFUNCTION()
  void OnOutOfAmmo();

  UFUNCTION(BlueprintCallable)
  bool GetCurrentWeaponAmmo(FAmmoWeapon &AmmoWeapon) const;

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Weapon")
  TSubclassOf<ALMABaseWeapon> WeaponClass;

  UPROPERTY(EditDefaultsOnly, Category = "Weapon")
  UAnimMontage *ReloadMontage;

  UPROPERTY()
  ALMABaseWeapon *Weapon = nullptr;

  virtual void BeginPlay() override;

public:
  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType,
                FActorComponentTickFunction *ThisTickFunction) override;
private:

  bool AnimReloading = false;

  void SpawnWeapon();
  void InitAnimNotify();

  void OnNotifyReloadFinished(USkeletalMeshComponent *SkeletalMesh);
  bool CanReload() const;
};