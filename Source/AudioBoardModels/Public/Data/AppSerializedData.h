#pragma once
#include "Events/Delegates.h"
#include "Models/ProjectModel.h"
#include "AppSerializedData.generated.h"

using FProjectModelFactory = TFunction<TObjectPtr<UProjectModel>(const FString& ProjectPath, UObject* Outer)>;


USTRUCT(BlueprintType)
struct FAppSerializedData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> AvailableProjectPaths;
};

using FAppStateLoader = TFunction<TFuture<TTuple<bool,FAppSerializedData>>(TSharedPtr<FThreadSafeBool> CancellationToken)>;

struct FAppModelInitArgs
{
	const FAppStateLoader Loader;
	const FProjectModelFactory ProjectFactory;
	const FFileOpenerFunction FileOpener;
};
