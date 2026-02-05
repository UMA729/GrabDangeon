// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearMenuGameMode.h"
#include "ClearHUD.h"

AClearMenuGameMode::AClearMenuGameMode()
{
	HUDClass = AClearHUD::StaticClass();
}
