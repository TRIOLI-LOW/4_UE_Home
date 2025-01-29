 // Fill out your copyright notice in the Description page of Project Settings.

#include "LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

ALMADefaultCharacter::ALMADefaultCharacter()
{

	PrimaryActorTick.bCanEverTick = true;
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed; // Базовая скорость
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false; // данное условие запрещаем камере вращаться относительно SpringArmComponent.

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CursorMaterial))
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
	

	OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);
}


// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  if (bIsSprinting) {
    if (CurrentStamina > 0) {
      CurrentStamina -= StaminaDrainRate * DeltaTime;
      if (CurrentStamina <= 0) {
        StopSprinting();
      }
    }
  } else {
    if (CurrentStamina < MaxStamina) {
      CurrentStamina += StaminaRegenRate * DeltaTime;
    }
  }

  if (!(HealthComponent->IsDead())) {
    RotationPlayerOnCursor();
  }
}
void ALMADefaultCharacter::RotationPlayerOnCursor() {
  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PC) {
    FHitResult ResultHit;
    PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
    float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(
                                     GetActorLocation(), ResultHit.Location)
                                     .Yaw;
    SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
    if (CurrentCursor) {
      CurrentCursor->SetWorldLocation(ResultHit.Location);
    }
  }
}
void ALMADefaultCharacter::OnDeath() {
  CurrentCursor->DestroyRenderState_Concurrent();

  PlayAnimMontage(DeathMontage);

  GetCharacterMovement()->DisableMovement();

  SetLifeSpan(5.0f);

  if (Controller) {
    Controller->ChangeState(NAME_Spectating);
  }
}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth) {
  GEngine->AddOnScreenDebugMessage(
      -1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Zoom", this, &ALMADefaultCharacter::ZoomCamera);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this,&ALMADefaultCharacter::StartSprinting);
    PlayerInputComponent->BindAction( "Sprint", IE_Released, this ,&ALMADefaultCharacter::StopSprinting);
}
void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}
void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}
void ALMADefaultCharacter::ZoomCamera(float Value) {
  if (SpringArmComponent) {

    float NewArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength - (Value * ZoomSpeed),
                                      MinArmLength, MaxArmLength);

    SpringArmComponent->TargetArmLength = NewArmLength;
	ArmLength = NewArmLength;
  }
}

//void ALMADefaultCharacter::Sprint(float Value) {
//}

void ALMADefaultCharacter::StartSprinting() {
  if (CurrentStamina > 0) {
    bIsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SpeedMultiplier;
  }
}

void ALMADefaultCharacter::StopSprinting() {
  bIsSprinting = false;
  GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
