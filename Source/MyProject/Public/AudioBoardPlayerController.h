// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AudioBoardPlayerController.generated.h"

UCLASS()
class MYPROJECT_API AAudioBoardPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual bool InputKey(const FInputKeyParams& Params) override;
};
