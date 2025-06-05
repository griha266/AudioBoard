// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FilePicker.h"
#include "Models/AppModel.h"
#include "Engine/GameInstance.h"
#include "AudioBoardGameInstance.generated.h"

/**
 * TODO
 * - Add sound import library
 * - Add mock sound import library
 * - Upload to github
 * - Connect to blueprints
 */
UCLASS()
class MYPROJECT_API UAudioBoardGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintPure, Category="Audio Board")
	[[nodiscard]] UAppModel* GetAppModel() const { return AppModel; }

private:
	UPROPERTY()
	TObjectPtr<UAppModel> AppModel = nullptr;

	TSharedPtr<FThreadSafeBool> CancellationToken = nullptr;
	TSharedPtr<SystemUtils::FFilePicker> FilePicker = nullptr;

	void InitAppModel();
};
