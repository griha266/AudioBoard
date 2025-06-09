// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FilePicker.h"
#include "InputEventsBus.h"
#include "Models/AppModel.h"
#include "Engine/GameInstance.h"
#include "AudioBoardGameInstance.generated.h"

UCLASS()
class MYPROJECT_API UAudioBoardGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintPure, Category="Audio Board")
	[[nodiscard]] UAppModel* GetAppModel() const noexcept { return AppModel; }
	UFUNCTION(BlueprintPure, Category="Audio Board|Input")
	[[nodiscard]] UInputEventsBus* GetInputEventsBus() const noexcept { return InputEventsBus; }

private:
	UPROPERTY()
	TObjectPtr<UAppModel> AppModel = nullptr;

	UPROPERTY()
	TObjectPtr<UInputEventsBus> InputEventsBus = nullptr;
	
	TSharedPtr<FThreadSafeBool> CancellationToken = nullptr;
	TSharedPtr<SystemUtils::FFilePicker> FilePicker = nullptr;

	void InitAppModel() noexcept;
};
