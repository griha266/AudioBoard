// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/AppModel.h"
#include "Utils/Initializable.h"


void UAppModel::Initialize(const FAppModelInitArgs& InitArgs)
{
	if (IsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("AppModel already initialized"));
		return;
	}
	AppStateLoader = InitArgs.Loader;
	ProjectFactory = InitArgs.ProjectFactory;
	FileOpener = InitArgs.FileOpener;
	IsInitialized = true;
}

TFuture<bool> UAppModel::LoadedAvailableProjects(TSharedPtr<FThreadSafeBool> CancellationToken)
{
	TPromise<bool> Operation;
	TFuture<bool> OperationResult = Operation.GetFuture();

	if (!AudioBoardUtils::IsReady(this))
	{
		Operation.SetValue(false);
	}
	else
	{
		if (ModelState == EModelState::Loading)
		{
			UE_LOG(LogTemp, Error, TEXT("Already loading projects"));
			Operation.SetValue(false);
		}
		else if (ModelState == EModelState::Loaded)
		{
			UE_LOG(LogTemp, Error, TEXT("Already loaded, recheck AppFlow"));
			Operation.SetValue(false);
		}
		else
		{
			// CT in app model should be null
			this->CancellationToken = CancellationToken;
			SetModelState(EModelState::Loading);

			// TODO replace with save game system
			// Combine cancellation tokens?
			AppStateLoader(CancellationToken)
				.Next([this, Operation = MoveTemp(Operation)](const TTuple<bool,FAppSerializedData>& Result) mutable
				{
					const auto IsSuccessful = Result.Key;
					if (!IsSuccessful || !this->CancellationToken.IsValid() || *(this->CancellationToken))
					{
						const TCHAR* Reason = IsSuccessful ? TEXT("Cancelled") : TEXT("Failed");
						UE_LOG(LogTemp, Warning, TEXT("Faild to upload: %s"), Reason);
						this->CancellationToken.Reset();
						SetModelState(EModelState::Unloaded);
						Operation.SetValue(false);
					}
					else
					{
						this->CancellationToken.Reset();
						this->SetModelState(EModelState::Loaded);
						const auto& Projects = Result.Value.AvailableProjectPaths;
						for (const auto& ProjectPath : Projects)
						{
							AvailableProjects.Add(this->ProjectFactory(ProjectPath, this));
						}
						Operation.SetValue(true);
						OnAvailableProjectsChanged.Broadcast(AvailableProjects);
					}
				});
		}
	}

	return OperationResult;
}

void UAppModel::Unload()
{
	if (!AudioBoardUtils::IsReady(this))
	{
		return;
	}

	if (ModelState == EModelState::Unloaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already unloaded"));
		return;
	}

	// We are importing now or loading project
	if (CancellationToken.IsValid())
	{
		// cancel current operation
		CancellationToken->AtomicSet(true);
		CancellationToken.Reset();
	}

	for (const auto& Project : AvailableProjects)
	{
		Project->Unload();
	}

	// Should be already unloaded
	if (IsValid(CurrentActiveProject))
	{
		CurrentActiveProject = nullptr;
		OnCurrentProjectChanged.Broadcast(nullptr);
	}

	AvailableProjects.Empty();
	OnAvailableProjectsChanged.Broadcast(AvailableProjects);
	SetModelState(EModelState::Unloaded);
}

void UAppModel::OpenExistingProject(UProjectModel* Project)
{
	if (!AudioBoardUtils::IsReady(this))
	{
		return;
	}

	if (ModelState != EModelState::Loaded)
	{
		UE_LOG(LogTemp, Error, TEXT("App model is not loaded yet!"));
		return;
	}

	if (Project == CurrentActiveProject)
	{
		UE_LOG(LogTemp, Error, TEXT("Project already opened or null"));
		return;
	}

	if (IsValid(CurrentActiveProject))
	{
		// IDK about that, feels like we need to wait first
		CurrentActiveProject->Unload();
		CurrentActiveProject = nullptr;
	}

	if (!IsValid(Project))
	{
		return;
	}

	SetModelState(EModelState::Loading);
	// we should not have any other ct right now, if we do, we have error
	CancellationToken = MakeShared<FThreadSafeBool>(false);
	
	Project->Load(CancellationToken).Next([this, Project](bool isProjectLoaded)
	{
		SetModelState(EModelState::Loaded);
		CancellationToken.Reset();
		if (isProjectLoaded)
		{
			CurrentActiveProject = Project;
			OnCurrentProjectChanged.Broadcast(CurrentActiveProject);
		}
	});
}


void UAppModel::AddOrCreateProject()
{
	if (!AudioBoardUtils::IsReady(this))
	{
		return;
	}

	if (ModelState != EModelState::Loaded)
	{
		UE_LOG(LogTemp, Error, TEXT("App model is not loaded"));
		return;
	}
	
	this->CancellationToken = MakeShared<FThreadSafeBool>(false);
	SetModelState(EModelState::Loading);
	FileOpener(CancellationToken).Next([this](const FString& Result)
	{
		const bool IsCancelled = (this->CancellationToken).Get();
		this->CancellationToken.Reset();
		if (IsCancelled || Result.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("AppModel could not be opened"));
			SetModelState(EModelState::Unloaded);
	 	}
		else
		{
			AvailableProjects.Add(this->ProjectFactory(Result, this));
			OnAvailableProjectsChanged.Broadcast(AvailableProjects);
			SetModelState(EModelState::Loaded);
		}
	});
}

void UAppModel::DeleteProject(UProjectModel* Project)
{
	if (!IsValid(Project))
	{
		UE_LOG(LogTemp, Error, TEXT("Project ptr is not valid"));
		return;
	}
	
	if (!AudioBoardUtils::IsReady(this))
	{
		return;
	}

	if (ModelState != EModelState::Loaded)
	{
		UE_LOG(LogTemp, Error, TEXT("App model is not loaded"));
		return;
	}

	if (IsValid(CurrentActiveProject))
	{
		// TODO should add state machine to prevent these cases
		UE_LOG(LogTemp, Error, TEXT("We should not be able to delete project from here, recheck app flow"));
		return;
	}


	for (auto Iterator = AvailableProjects.CreateIterator(); Iterator; ++Iterator)
	{
		if (*Iterator == Project)
		{
			(*Iterator)->Unload();
			Iterator.RemoveCurrent();
			OnAvailableProjectsChanged.Broadcast(AvailableProjects);
			break;
		}
	}
}

void UAppModel::ToggleListeningKeyBind()
{
	IsListeningKeyBind = !IsListeningKeyBind;
	OnListeningKeyBindChanged.Broadcast(IsListeningKeyBind);
}

// GETTERS

const TArray<UProjectModel*>& UAppModel::GetAvailableProjects() const
{
	return AvailableProjects;
}

UProjectModel* UAppModel::GetCurrentLoadedProject() const
{
	if (IsValid(CurrentActiveProject))
	{
		return CurrentActiveProject;
	}

	return nullptr;
}

bool UAppModel::GetIsInitialized() const
{
	return IsInitialized;
}

bool UAppModel::GetIsListeningKeyBind() const
{
	return IsListeningKeyBind;
}

// PRIVATE

void UAppModel::SetModelState(const EModelState NewModelState)
{
	if (NewModelState != ModelState)
	{
		ModelState = NewModelState;
		OnModelStateChanged.Broadcast(ModelState);
	}
}
