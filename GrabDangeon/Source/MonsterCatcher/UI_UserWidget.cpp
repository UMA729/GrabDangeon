// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_UserWidget.h"
#include "MyGameInstance.h"

void UUI_UserWidget::SetHPPercent(float Percent)
{
    if (HPBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("%f"),Percent);
        HPBar->SetPercent(Percent);
    }
}

void UUI_UserWidget::SetKeyCount(int32 key_count)
{
    if (KeyCount)
    {
        KeyCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), key_count)));
        if (key_count == 0)
        {
            KeyBackImg->SetFillColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 0.1f)); 
            KeyImg->SetRenderOpacity(0.25f);
        }
        else
        {
            KeyBackImg->SetFillColorAndOpacity(FLinearColor(0.5f, 0.75f, 0.f, 1.f));
            KeyImg->SetRenderOpacity(1.0f);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("not text box"));
    }
}
