// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DoorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERCATCHER_API UDoorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    // ドアが開いているか
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    bool bIsOpen = false;

    // 開閉用アルファ（0〜1）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    float OpenAlpha = 0.0f;
};
