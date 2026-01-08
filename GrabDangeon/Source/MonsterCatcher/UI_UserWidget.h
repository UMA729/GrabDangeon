// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TEXTBlock.h"
#include "Components/Image.h"
#include "UI_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERCATCHER_API UUI_UserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HPBar;
    
    UPROPERTY(meta = (BindWidget))
    UProgressBar* KeyBackImg;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KeyCount;

    UPROPERTY(meta = (BindWidget))
    UImage* KeyImg;

    UFUNCTION(BlueprintCallable)
    void SetHPPercent(float percent);

    UFUNCTION(BlueprintCallable)
    void SetKeyCount(int32 key_count);

};
