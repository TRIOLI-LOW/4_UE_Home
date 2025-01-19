// Fill out your copyright notice in the Description page of Project Settings.


#include "LMAGameMode.h"
#include "LMAPlayerController.h"
#include "LMADefaultCharacter.h"



ALMAGameMode::ALMAGameMode() {

	DefaultPawnClass = ALMADefaultCharacter::StaticClass();
	PlayerControllerClass = ALMAPlayerController::StaticClass();
}
