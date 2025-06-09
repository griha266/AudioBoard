// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/AppSerializedData.h"
#include "Events/Delegates.h"
#include "Data/ModelState.h"
#include "Models/ProjectModel.h"
#include "UObject/Object.h"
#include "AppModel.generated.h"


// Do not forget to bind only on game logic
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentProjectModelChanged, UProjectModel*, NewProject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAvailableProjectsChangedDelegate, const TArray<UProjectModel*>&, AvailableProjects);

/**
 * Model of whole application, contains all project models
 * Should be initialized and loaded first before used
 */
UCLASS(BlueprintType, Transient)
class AUDIOBOARDMODELS_API UAppModel : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Analog for constructor with arguments
	 * @param InitArgs Initialization arguments for this model
	 */
	void Initialize(const FAppModelInitArgs& InitArgs);
	
	/**
	 * Start loading last used projects from cache file path
	 * Return false if called while already loading
	 * @param CancellationToken for cancelling load operation
	 * @return Success of the operation
	 */
	TFuture<bool> LoadedAvailableProjects(TSharedPtr<FThreadSafeBool> CancellationToken);
	
	/**
	 * Unload existing project models
	 * If called in the process of loading, cancel operation and unload existing models
	 */
	void Unload();

	/**
	 * Sets project form existing project models in @link AvailableProjects @endlink
	 * as current and starts to load it
	 * @param Project Ptr on existing project from @link AvailableProjects @endlink
	 */
	UFUNCTION(BlueprintCallable, Category = "App events", meta = (ShortTooltip = "Open existing project"))
	void OpenExistingProject(UProjectModel* Project);
	
	/**
	 * Try to open file picker to select or create new project file
	 * This does not load project, just adds to the current list
	 */
	UFUNCTION(BlueprintCallable, Category = "App events", meta = (ShortTooltip = "Create or add new project"))
	void AddOrCreateProject();
	
	/**
	 * Delete project if exist in @link AvailableProjects @endlink 
	 * @param Project Ptr on existing project form @link AvailableProjects @endlink 
	 */
	UFUNCTION(BlueprintCallable, Category = "App events", meta = (ShortTooltip = "Delete existing project"))
	void DeleteProject(UProjectModel* Project);
	
	UFUNCTION(BlueprintCallable, Category = "App events")
	void ToggleListeningKeyBind();

	// GETTERS
	/**
	 * @return Current available projects on this model
	 */
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
    [[nodiscard]] const TArray<UProjectModel*>& GetAvailableProjects() const;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] UProjectModel* GetCurrentLoadedProject() const;

	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] bool GetIsInitialized() const;
	
	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] bool GetIsListeningKeyBind() const;

	UFUNCTION(BlueprintPure, Category = "Data|Getters")
	[[nodiscard]] EModelState GetModelState() const;

	// EVENTS
	/**
	 * Called when current loaded project was changed, even if it was set to nullptr
	 */
	UPROPERTY(BlueprintAssignable, Category = "App events")
	FOnCurrentProjectModelChanged OnCurrentProjectChanged;

	/**
	 * Called when AvailableProjects collection was changed, even if it becomes empty
	 */
	UPROPERTY(BlueprintAssignable, Category = "App events")
	FOnAvailableProjectsChangedDelegate OnAvailableProjectsChanged;

	
	UPROPERTY(BlueprintAssignable, Category = "App events")
	FOnModelStateChangedDelegate OnModelStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "App events")
	FOnModelBoolFieldChangedDelegate OnListeningKeyBindChanged;
	
private:
	// INIT fields
	bool IsInitialized = false;
	bool IsListeningKeyBind = false;
	FAppStateLoader AppStateLoader = nullptr;;
	FProjectModelFactory ProjectFactory = nullptr;
	FFileOpenerFunction FileOpener = nullptr;

	// Runtime fields
	EModelState ModelState = EModelState::Unloaded;
	// Should be null if ModelState != EModelState::Loading
	TSharedPtr<FThreadSafeBool> CancellationToken = nullptr;
	
	UPROPERTY()
	TArray<UProjectModel*> AvailableProjects;

	UPROPERTY()
	TObjectPtr<UProjectModel> CurrentActiveProject = nullptr;

	void SetModelState(EModelState NewModelState);
};


