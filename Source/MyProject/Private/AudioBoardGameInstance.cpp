// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject/Public/AudioBoardGameInstance.h"

#include "ModelFactories.h"
#include "AudioBoardSettings.h"

/*
 * TODO list
 * - How to feed mock data from the editor?
 *	- We need some blueprint or whatever
 * - Import runtime audio import plugin
 * - Clean up code
 *  - Recheck your usage of pointers
 *  - Recheck constructors (rule of 5)
 *  - Recheck access modifiers
 */

FAppModelInitArgs MakeRuntimeInitArgs(const UAudioBoardSettings* const InSettings)
{
	// TODO implement, 
	return FAppModelInitArgs();
}

FAppModelInitArgs MakeMockInitArgs()
{
	return FAppModelInitArgs(
			&MocksUtils::LoadMockAppFile,
			&ModelFactories::ProjectModelFactory,
			&MocksUtils::FileOpener
	);
}


void UAudioBoardGameInstance::Init()
{
	Super::Init();
	const auto Config = GetDefault<UAudioBoardSettings>();
	
	FilePicker = MakeShared<SystemUtils::FFilePicker>(
		Config->DefaultOpenFolderPath,
		Config->ProjectFileFilter
	);
	InputEventsBus = NewObject<UInputEventsBus>(this);
	InitAppModel();
	UE_LOG(LogTemp, Display, TEXT("Audio Board Initialized"));
}

void UAudioBoardGameInstance::Shutdown()
{
	CancellationToken->AtomicSet(false);
	CancellationToken.Reset();
	FilePicker.Reset();
	UE_LOG(LogTemp, Display, TEXT("Audio Board Shutdown"));
	InputEventsBus = nullptr;
	AppModel->Unload();
	AppModel = nullptr;
	Super::Shutdown();
}

void UAudioBoardGameInstance::InitAppModel() noexcept
{
	CancellationToken = MakeShared<FThreadSafeBool>(false);
	AppModel = NewObject<UAppModel>(this);
	const auto InSettings = GetDefault<UAudioBoardSettings>();
	if (InSettings->UseMock)
	{
		AppModel->Initialize(MakeMockInitArgs());
	}
	else
	{
		AppModel->Initialize(MakeRuntimeInitArgs(InSettings));
	}
	AppModel->LoadedAvailableProjects(CancellationToken);
}