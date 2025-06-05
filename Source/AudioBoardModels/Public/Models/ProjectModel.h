// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioModel.h"
#include "Events/Delegates.h"
#include "Data/ModelState.h"
#include "Data/ProjectSerializedData.h"
#include "UObject/Object.h"
#include "ProjectModel.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAudioCollectionChangedDelegate, const TArray<UAudioModel*>&, AudioModels);

/**
 * TODO how to make destructor for UObjects?
 * Should I unbind delegates? Or its handled by gc?
 */
UCLASS(BlueprintType, NotBlueprintable, Transient)
class AUDIOBOARDMODELS_API UProjectModel : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const FProjectModelInitArgs& InitArgs);
	TFuture<bool> Load(TSharedPtr<FThreadSafeBool> LoadingCT);
	void Unload();
	
	[[nodiscard]] bool GetIsInitialized() const;

	UFUNCTION(BlueprintPure, Category="Data|Getters")
	[[nodiscard]] const TArray<UAudioModel*>& GetAudioModels() const;

	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] const FString& GetFilePath() const;

	UPROPERTY(BlueprintAssignable, Category = "Data|Events")
	FOnModelStateChangedDelegate OnModelStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Data|Events")
	FOnAudioCollectionChangedDelegate OnAudioCollectionChanged;
	
private:
	bool IsInitialized = false;
	FString ProjectFilePath = TEXT("");
	FAudioModelFactory AudioModelFactory = nullptr;
	TFileLoaderFunction<FProjectSerializedData> FileLoader = nullptr;
	
	UPROPERTY()
	TArray<UAudioModel*> AudioModels;

	EModelState ModelState = EModelState::Unloaded;
	// Should exist only if ModelState == Loading
	TSharedPtr<FThreadSafeBool> LoadingCancellationToken = nullptr;
	
	void SetModelState(const EModelState NewModelState);

};
