
#pragma once

#include "CoreMinimal.h"
#include "Data/AudioSerializedData.h"
#include "Events/Delegates.h"
#include "Data/ModelState.h"
#include "Models/SoundPlayer.h"
#include "UObject/Object.h"
#include "AudioModel.generated.h"



// TODO define destructor
// TODO how to make reactive property in Unreal?
UCLASS(BlueprintType, NotBlueprintable, Transient)
class AUDIOBOARDMODELS_API UAudioModel : public UObject
{
	GENERATED_BODY()

public:
	void Init(const FAudioModelInitData& InitialData);
	void Load();
	void Unload();
	void Play() const;
	void Stop() const;
	
	[[nodiscard]] bool GetIsInitialized() const;

	UFUNCTION(BlueprintCallable, Category = "Data|Setters")
	void SetIsLooped(const bool NewValue) const;
	
	UFUNCTION(BlueprintCallable, Category = "Data|Setters")
	void SetKeyBind(const FKey& NewValue);
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] const USoundPlayer* GetPlayer() const;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] const FKey& GetKey() const;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] EModelState GetModelState() const;
	

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

	void SetModelState(const EModelState NewValue);
};
