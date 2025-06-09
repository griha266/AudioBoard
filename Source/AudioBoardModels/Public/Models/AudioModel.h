
#pragma once

#include "CoreMinimal.h"
#include "Data/AudioSerializedData.h"
#include "Events/Delegates.h"
#include "Data/ModelState.h"
#include "Models/SoundPlayer.h"
#include "UObject/Object.h"
#include "AudioModel.generated.h"



UCLASS(BlueprintType, Transient)
class AUDIOBOARDMODELS_API UAudioModel : public UObject
{
	GENERATED_BODY()

public:
	void Init(const FAudioModelInitData& InitialData) noexcept;
	void Load() noexcept;
	void Unload() noexcept;
	
	[[nodiscard]] bool GetIsInitialized() const noexcept;

	UFUNCTION(BlueprintCallable, Category = "Audio events")
	void Play() const noexcept;
	
	UFUNCTION(BlueprintCallable, Category = "Audio events")
	void Stop() const noexcept;
	
	UFUNCTION(BlueprintCallable, Category = "Data|Setters")
	void SetIsLooped(const bool NewValue) const noexcept;
	
	UFUNCTION(BlueprintCallable, Category = "Data|Setters")
	void SetKeyBind(const FKey& NewValue) noexcept;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] const USoundPlayer* GetPlayer() const noexcept;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] const FKey& GetKey() const noexcept;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] EModelState GetModelState() const noexcept;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] bool GetIsPlaying() const noexcept;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] bool GetIsLooped() const noexcept;

	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] const FString& GetFilePath() const noexcept;
	
	

	UPROPERTY(BlueprintAssignable, Category = "Data|Events")
	FOnModelKeyFieldChangedDelegate OnKeyChanged;

	UPROPERTY(BlueprintAssignable, Category = "Data|Events")
	FOnModelStateChangedDelegate OnModelStateChanged;
	
private:
	bool IsInitialized = false;
	FString FilePath = TEXT("");
	FAudioImporter AudioImporter = nullptr;
	EModelState ModelState = EModelState::Unloaded;
	FKey KeyBind = FKey(); // EKey::Invalid does not work on arm64
	
	TSharedPtr<FThreadSafeBool> ImportingCT = nullptr;
	
	UPROPERTY()
	TObjectPtr<USoundPlayer> Player = nullptr;

	void SetModelState(const EModelState NewValue) noexcept;
};
