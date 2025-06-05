#pragma once

#include "Models/AudioModel.h"
#include "Events/Delegates.h"
#include "ProjectSerializedData.generated.h"

using FAudioModelFactory = TFunction<TObjectPtr<UAudioModel>(const FAudioSerializedData& Data, UObject* Outer)>;

USTRUCT(BlueprintType)
struct FProjectSerializedData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAudioSerializedData> AudioSerializedData;
};

struct FProjectModelInitArgs
{
	const FString ProjectPath;
	const FAudioModelFactory AudioModelFactory;
	const TFileLoaderFunction<FProjectSerializedData> FileLoader;
};
