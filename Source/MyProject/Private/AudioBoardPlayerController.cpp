// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioBoardPlayerController.h"

#include "AudioBoardGameInstance.h"
#include "GameFramework/PlayerInput.h"


bool AAudioBoardPlayerController::InputKey(const FInputKeyParams& Params)
{
	const bool Result = Super::InputKey(Params);
	const UInputEventsBus* KeyBus = GetGameInstance<UAudioBoardGameInstance>()->GetInputEventsBus();
	if (IsValid(KeyBus))
	{
		KeyBus->HandleKeyDown(Params.Key);
	}
	return Result;
}
