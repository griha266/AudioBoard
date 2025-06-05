// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/Delegates.h"
#include "UObject/Object.h"
#include "SoundPlayer.generated.h"


UCLASS(BlueprintType, NotBlueprintable, Transient)
class AUDIOBOARDMODELS_API USoundPlayer : public UObject
{
	GENERATED_BODY()
public:
	void Init(int MaxConcurrentSounds);
	void SetIsLooped(bool bIsLooped);
	void SetSound(TObjectPtr<USoundWave> SoundWave);
	void ClearSound();
	bool Play();
	void Stop();

	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] bool GetIsReadyToPlay() const noexcept;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] FORCEINLINE bool GetIsPlaying() const noexcept
	{
		return bIsPlaying;
	}
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] bool GetIsLooped() const noexcept
	{
		return bIsLooped;
	}

	UPROPERTY(BlueprintAssignable, Category = "Data|Events")
	FSimpleDynamicMulticastDelegate OnAudioFinished;
	
	UPROPERTY(BlueprintAssignable, Category = "Data|Events")
	FOnModelBoolFieldChangedDelegate OnIsLoopedChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Data|Events")
	FOnModelBoolFieldChangedDelegate OnIsPlayingChanged;

private:
	int MaxConcurrentSounds = 0;
	float AudioDuration = 0;
	bool bIsLooped = false;
	bool bIsPlaying = false;
	
	TArray<FTimerHandle> CurrentPlaybackHandles = TArray<FTimerHandle>();

	UPROPERTY()
	TObjectPtr<USoundWave> SoundWave = nullptr;

	void SetIsPlaying(bool bIsPlaying);
	void AudioFinishedHandle(const TSharedPtr<const FTimerHandle> PlaybackHandle);
	TSharedPtr<const FTimerHandle> StartPlayingSound();
	
};
