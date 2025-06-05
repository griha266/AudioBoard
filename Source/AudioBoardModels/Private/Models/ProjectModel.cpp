#include "Models/ProjectModel.h"

#include "Utils/Initializable.h"

void UProjectModel::Initialize(const FProjectModelInitArgs& InitArgs)
{
	if (IsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectModel: Already initialized"));
		return;
	}

	ProjectFilePath = InitArgs.ProjectPath;
	AudioModelFactory = InitArgs.AudioModelFactory;
	FileLoader = InitArgs.FileLoader;
	IsInitialized = true;
}

TFuture<bool> UProjectModel::Load(TSharedPtr<FThreadSafeBool> LoadingCT)
{
	TPromise<bool> Operation;
	TFuture<bool> Result = Operation.GetFuture();
	
	if (!AudioBoardUtils::IsReady(this))
	{
		Operation.SetValue(false);
	}
	else if (ModelState != EModelState::Unloaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Project model is already loading or loaded"));
		Operation.SetValue(false);
	} 
	else
	{
		LoadingCancellationToken = LoadingCT;
		SetModelState(EModelState::Loading);
		FileLoader(ProjectFilePath, LoadingCancellationToken)
			.Next([this, Operation = MoveTemp(Operation)](const TTuple<bool,FProjectSerializedData>& Result) mutable
			{
				const bool IsSuccessful = Result.Key;
				if (!IsSuccessful || !LoadingCancellationToken.IsValid() || *LoadingCancellationToken)
				{
					const TCHAR* Reason = IsSuccessful ? TEXT("Cancelled") : TEXT("Failed");
					UE_LOG(LogTemp, Warning, TEXT("Failed to load project, reason: %s"), Reason);
					SetModelState(EModelState::Unloaded);
					LoadingCancellationToken.Reset();
					Operation.SetValue(false);
				}
				else
				{
					for (const auto& AudioSerializedData : Result.Value.AudioSerializedData)
					{
						TObjectPtr<UAudioModel> AudioModel = AudioModelFactory(AudioSerializedData, this);
						AudioModels.Push(AudioModel);
					}
					
					LoadingCancellationToken.Reset();
					OnAudioCollectionChanged.Broadcast(AudioModels);
					SetModelState(EModelState::Loaded);
					Operation.SetValue(true);
				}
				
			});
	}

	return Result;
}

void UProjectModel::Unload()
{
	if (!AudioBoardUtils::IsReady(this))
	{
		return;
	}

	if (ModelState == EModelState::Unloaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Project model is already unloaded"));
		return;
	}

	if (ModelState == EModelState::Loading)
	{
		LoadingCancellationToken->AtomicSet(false);
		LoadingCancellationToken.Reset();
	}

	for (const auto& AudioModel : AudioModels)
	{
		AudioModel->Unload();
	}
	
	AudioModels.Empty();
	OnAudioCollectionChanged.Broadcast(AudioModels);
	SetModelState(EModelState::Unloaded);
}

const TArray<UAudioModel*>& UProjectModel::GetAudioModels() const
{
	return AudioModels;
}

const FString& UProjectModel::GetFilePath() const
{
	return ProjectFilePath;
}

bool UProjectModel::GetIsInitialized() const
{
	return IsInitialized;
}


void UProjectModel::SetModelState(const EModelState NewModelState)
{
	if (ModelState != NewModelState)
	{
		ModelState = NewModelState;
		OnModelStateChanged.Broadcast(ModelState);
	}
}
