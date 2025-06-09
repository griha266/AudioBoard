#pragma once
#include "AudioBoardSettings.h"
#include "Data/AppSerializedData.h"

namespace MocksUtils
{
	[[nodiscard]] static TFuture<TTuple<bool, FAppSerializedData>> LoadMockAppFile(TSharedPtr<FThreadSafeBool> CancellationToken)
	{
		TPromise<TTuple<bool, FAppSerializedData>> Operation;
		UE_LOG(LogTemp, Display, TEXT("LoadMockAppFile with path"));
		const auto ProjectConfig = GetDefault<UAudioBoardSettings>();
		
		FAppSerializedData Data = ProjectConfig->MockAppSerializedData.ToAppSerializeData();
		Operation.SetValue(MakeTuple(false, Data));
		return Operation.GetFuture();
	}
	
	[[nodiscard]] static TFuture<TTuple<bool, FProjectSerializedData>> LoadMockProject(const FString& Path, TSharedPtr<FThreadSafeBool> CancellationToken)
	{
		UE_LOG(LogTemp, Display, TEXT("LoadMockProject at path : %s"), *Path);
		TPromise<TTuple<bool, FProjectSerializedData>> Operation;
		const auto MockAppData = GetDefault<UAudioBoardSettings>()->MockAppSerializedData;
		if (!MockAppData.MockProjects.Contains(Path))
		{
			UE_LOG(LogTemp, Display, TEXT("Failed to load mock on path %s"), *Path);
			FProjectSerializedData Data;
			Operation.SetValue(MakeTuple(false, Data));
		}
		else
		{
			FProjectSerializedData Data(
				MockAppData.GetMockAudios()
			);
			
			Operation.SetValue(MakeTuple(true, Data));
		}
		
		return Operation.GetFuture();
	}
	
	[[nodiscard]] static TFuture<FString> FileOpener(TSharedPtr<FThreadSafeBool> CancellationToken)
	{
		TPromise<FString> Operation;
		UE_LOG(LogTemp, Display, TEXT("Open file mock"));
		const auto FilePickerResult = GetDefault<UAudioBoardSettings>()->MockFilePickerResult;
		Operation.SetValue(FilePickerResult);
		return Operation.GetFuture();
	}

	[[nodiscard]] static TFuture<TObjectPtr<USoundWave>> LoadSoundWave(const FString& FilePath, TSharedPtr<FThreadSafeBool> CancellationToken)
	{
		UE_LOG(LogTemp, Display, TEXT("Load sound wave at : %s"), *FilePath);
		TPromise<TObjectPtr<USoundWave>> Operation;
		TFuture<TObjectPtr<USoundWave>> OperationResult = Operation.GetFuture();

		const auto MockAppData = GetDefault<UAudioBoardSettings>()->MockAppSerializedData;
		FMockAudioSerializedData ResultAudio;
		if (!MockAppData.FindAudio(FilePath, ResultAudio))
		{
			UE_LOG(LogTemp, Display, TEXT("Failed to find mock audio with path : %s"), *FilePath);
			Operation.SetValue(nullptr);
		}
		else
		{
			const auto AudioPtr = ResultAudio.SoundWave;
			const auto Asset = AudioPtr.LoadSynchronous();
			if (IsValid(Asset))
			{
				Operation.SetValue(ToObjectPtr<USoundWave>(Asset));
			}
			else
			{
				Operation.SetValue(nullptr);
			}
			// TODO implement async support
			// FStreamableManager& LoadManager = UAssetManager::GetStreamableManager();
			// LoadManager.RequestAsyncLoad(
			// 	AudioPtr.ToSoftObjectPath(),
			// 	FStreamableDelegate::CreateLambda([Operation = MoveTemp(Operation), AudioPtr]() mutable
			// 	{
			// 		if (AudioPtr.IsValid())
			// 		{
			// 			Operation.SetValue(ToObjectPtr(AudioPtr.Get()));
			// 		} else
			// 		{
			// 			UE_LOG(LogTemp, Display, TEXT("Failed to load mock audio"));
			// 			Operation.SetValue(nullptr);
			// 		}
			// 	})
			// );
		}
		return OperationResult;
	}
}
