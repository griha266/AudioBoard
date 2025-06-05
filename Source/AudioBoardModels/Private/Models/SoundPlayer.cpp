// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/SoundPlayer.h"

#include "Kismet/GameplayStatics.h"


void USoundPlayer::Init(int MaxConcurrentSounds)
{
	this->MaxConcurrentSounds = MaxConcurrentSounds;
}

void USoundPlayer::SetIsLooped(bool bIsLooped)
{
	if (this->bIsLooped != bIsLooped)
	{
		this->bIsLooped = bIsLooped;
		OnIsLoopedChanged.Broadcast(bIsLooped);
	}
}

void USoundPlayer::SetSound(TObjectPtr<USoundWave> Sound)
{
	if (!IsValid(Sound))
	{
		UE_LOG(LogTemp, Error, TEXT("Sound is invalid"));
		return;
	}
	
    Stop();

	SoundWave = Sound;
	SoundWave->bLooping = false;
	AudioDuration = SoundWave->GetDuration();
}

void USoundPlayer::ClearSound()
{
	if (!IsValid(SoundWave))
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundWave is invalid"));
		return;
	}
	
    Stop();
    SoundWave = nullptr;
}

bool USoundPlayer::Play()
{
	if (!GetIsReadyToPlay())
	{
		UE_LOG(LogTemp, Error, TEXT("Player is not ready to play, Do you forgot to set the sound wave?"));
		return false;
	}
	
	if (CurrentPlaybackHandles.Num() >= MaxConcurrentSounds)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot play sound due to full playback"));
		return false;
	}

	SetIsPlaying(true);
	const auto PlaybackHandle = StartPlayingSound();
	CurrentPlaybackHandles.Add(*PlaybackHandle);
	return true;
}

void USoundPlayer::Stop()
{
	if (!GetIsPlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("No active sound playbacks"));
		return;
	}

	for (auto Iterator = CurrentPlaybackHandles.CreateIterator(); Iterator; ++Iterator)
	{
		GetWorld()->GetTimerManager().ClearTimer(*Iterator);
		Iterator.RemoveCurrent();
	}

	SetIsPlaying(false);
	OnAudioFinished.Broadcast();
}

bool USoundPlayer::GetIsReadyToPlay() const noexcept
{
	return IsValid(SoundWave);
}


void USoundPlayer::SetIsPlaying(bool bIsPlaying)
{
	if (this->bIsPlaying != bIsPlaying)
	{
		this->bIsPlaying = bIsPlaying;
		OnIsPlayingChanged.Broadcast(bIsPlaying);
	}
}

// TODO possible bug if handle does not exist in array
void USoundPlayer::AudioFinishedHandle(const TSharedPtr<const FTimerHandle> PlaybackHandle)
{
	CurrentPlaybackHandles.RemoveSingle(*PlaybackHandle);
	
	auto PlayingAnotherLoop = bIsLooped && Play();
	
	if (!PlayingAnotherLoop && CurrentPlaybackHandles.Num() == 0)
	{
		SetIsPlaying(false);
		OnAudioFinished.Broadcast();
	}
}

TSharedPtr<const FTimerHandle> USoundPlayer::StartPlayingSound()
{
	TSharedPtr<FTimerHandle> PlaybackHandle = MakeShared<FTimerHandle>();
	UGameplayStatics::PlaySound2D(this, SoundWave);
	GetWorld()->GetTimerManager().SetTimer(
		*PlaybackHandle,
		FTimerDelegate::CreateLambda([t = this, PlaybackHandle]()
		{
			if (IsValid(t))
			{
				t->AudioFinishedHandle(PlaybackHandle);
			}
		}),
		AudioDuration,
		false
	);
	
	return PlaybackHandle;
}