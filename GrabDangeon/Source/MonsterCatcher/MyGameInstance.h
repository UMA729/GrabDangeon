// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERCATCHER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	void Initialize();

	int32 keycount = 0;

};
