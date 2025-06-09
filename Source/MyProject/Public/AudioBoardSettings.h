// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MockSerializedData.h"
#include "Engine/DeveloperSettings.h"
#include "AudioBoardSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, BlueprintType)
class MYPROJECT_API UAudioBoardSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Audio Board Settings")
	bool UseMock;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Audio Board Settings|Prod Data")
	FString DefaultOpenFolderPath;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Audio Board Settings|Prod Data")
	FString ProjectFileFilter;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Audio Board Settings|Mock Data")
	FMockSerializedData MockAppSerializedData;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Audio Board Settings|Mock Data")
	FString MockFilePickerResult;
};
