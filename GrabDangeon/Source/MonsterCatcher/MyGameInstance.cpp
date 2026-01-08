// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::Initialize()
{
	const UMyGameInstance* DefaultObject = GetDefault<UMyGameInstance>();

	this->keycount = DefaultObject->keycount;
}