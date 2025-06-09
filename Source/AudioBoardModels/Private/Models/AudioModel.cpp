// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/AudioModel.h"

#include "Utils/Initializable.h"


void UAudioModel::Init(const FAudioModelInitData& InitialData) noexcept
{
	if (IsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("AudioModel already initialized"));
		return;
	}

	KeyBind = InitialData.SerializedData.KeyBind;
	AudioImporter = InitialData.AudioImporter;
	FilePath = InitialData.SerializedData.FilePath;
	Player = NewObject<USoundPlayer>(this);
	Player->Init(10);
	Player->SetIsLooped(InitialData.SerializedData.IsLooped);
	IsInitialized = true;
}

void UAudioModel::Play() const noexcept
{
	if (!AudioBoardUtils::IsReady(this) || ModelState != EModelState::Loaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Model is not ready to play audio!"));
		return;
	}

	if (Player->GetIsLooped() && Player->GetIsPlaying())
	{
            Player->Stop();
	}
	else if (!Player->Play())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to play audio!"));
	}
}

void UAudioModel::Stop() const noexcept
{
	if (!AudioBoardUtils::IsReady(this) || ModelState != EModelState::Loaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Model is not ready to play audio!"));
	}

	Player->Stop();
}


void UAudioModel::Load() noexcept
{
	if (!AudioBoardUtils::IsReady(this))
	{
		return;
	}

	// If we are playing, we are loaded, so that case also covered
	if (ModelState != EModelState::Unloaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("AudioModel already loaded or loading"));
		return;
	}

	ImportingCT = MakeShared<FThreadSafeBool>(false);
	SetModelState(EModelState::Loading);
	AudioImporter(FilePath, ImportingCT)
		.Next([this](auto Result)
		{
			bool IsSuccessful = IsValid(Result);
			// We cancel operation
			if (!IsSuccessful || !ImportingCT.IsValid() || *ImportingCT)
			{
				const TCHAR* Reason = IsSuccessful ? TEXT("Cancelled") : TEXT("Failed");
				UE_LOG(LogTemp, Warning, TEXT("Faild to load, reason: %s"), Reason);
				SetModelState(EModelState::Unloaded);
				ImportingCT.Reset();
				return;
			}

			Player->SetSound(Result);
			ImportingCT.Reset();
			SetModelState(EModelState::Loaded);
		});
}

void UAudioModel::Unload() noexcept
{
	if (!AudioBoardUtils::IsReady(this))
	{
		return;
	}
	
	if (ModelState == EModelState::Unloaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("AudioModel already unloaded"));
		return;
	}

	if (ModelState == EModelState::Loading)
	{
		ImportingCT->AtomicSet(true);
		ImportingCT.Reset();
	}

	Player->ClearSound();
	SetModelState(EModelState::Unloaded);
}

// Reactive properties
bool UAudioModel::GetIsInitialized() const noexcept
{
	return IsInitialized;
}

const USoundPlayer* UAudioModel::GetPlayer() const noexcept
{
	return Player;
}


void UAudioModel::SetIsLooped(const bool NewValue) const noexcept
{
	if (!AudioBoardUtils::IsReady(this))
	{
		return;
	}

	Player->SetIsLooped(NewValue);
}


void UAudioModel::SetKeyBind(const FKey& NewValue) noexcept
{
	if(AudioBoardUtils::IsReady(this))
	{
		return;
	}
	if (NewValue != KeyBind)
	{
		KeyBind = NewValue;
		OnKeyChanged.Broadcast(NewValue);
	}
}

const FKey& UAudioModel::GetKey() const noexcept
{
	return KeyBind;
}

EModelState UAudioModel::GetModelState() const noexcept
{
	return ModelState;
}

bool UAudioModel::GetIsPlaying() const noexcept
{
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("Model is not initialized yet"));
		return false;
	}
	return Player->GetIsPlaying();
}

bool UAudioModel::GetIsLooped() const noexcept
{
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("Model is not initialized yet"));
		return false;
	}
	return Player->GetIsLooped();
}

const FString& UAudioModel::GetFilePath() const noexcept
{
	return FilePath;
}

// PRIVATE methods
void UAudioModel::SetModelState(const EModelState NewValue) noexcept
{
	if (NewValue != ModelState)
	{
		ModelState = NewValue;
		OnModelStateChanged.Broadcast(ModelState);
	}
}

