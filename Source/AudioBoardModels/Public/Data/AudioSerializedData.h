#pragma once
#include "AudioSerializedData.generated.h"

using FAudioImporter = TFunction<TFuture<TObjectPtr<USoundWave>>(const FString&, TSharedPtr<FThreadSafeBool>)>;

USTRUCT(BlueprintType)
struct FAudioSerializedData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FilePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLooped;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey KeyBind;
};

struct FAudioModelInitData
{
	const FAudioSerializedData SerializedData;
	const FAudioImporter AudioImporter;
};
