// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent; // ����� �������� �� ��������� ������.
class USpringArmComponent; // ������������ ��� ��������������� ���������� ���������� ������ ����������, ����� ��� ���������� ��������.
class ULMAHealthComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter

{ 
	GENERATED_BODY()

	// Sets default values for this character's properties
	ALMADefaultCharacter();
public:
    UFUNCTION()
    ULMAHealthComponent *GetHealthComponent() const { return HealthComponent;}

      protected:

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathMontage;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
    ULMAHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	virtual void BeginPlay() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
    void ZoomCamera(float Value);

    float MinArmLength = 500.0f; 
    float MaxArmLength = 2000.0f; 
    float ZoomSpeed = 100.0f; 

	float YRotation = -75.0f; // �������� �� ������� ������ �� ��� Y.
	float ArmLength = 1400.0f; // �������� �� ����� �������.
	float FOV = 55.0f; // �������� �� ���� ������ ������.

	void OnDeath();
    void OnHealthChanged(float NewHealth);
    void RotationPlayerOnCursor();
      public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
