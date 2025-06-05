#pragma once
#include "Data/AppSerializedData.h"
#include "MockSerializedData.generated.h"

namespace MockData {
	template<typename TInput, typename TOutput>
	[[nodiscard]] static TArray<TOutput> MapCopy(const TArray<TInput>& InArray, const TFunction<TOutput(const TInput& Data)>& Mapper)
	{
		TArray<TOutput> OutArray = TArray<TOutput>();
		OutArray.Reserve(InArray.Num()); // TODO Probably there are a constructor for that
		for (int32 i = 0; i < InArray.Num(); i++)
		{
			auto Item = Mapper(InArray[i]);
			OutArray[i] = Item;
		}

		return OutArray;
	}
}

USTRUCT(BlueprintType)
struct FMockAudioSerializedData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAudioSerializedData AudioSerializedData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundWave> SoundWave;
};


USTRUCT(BlueprintType)
struct FMockSerializedData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> MockProjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMockAudioSerializedData> MockAudioList;
	
	[[nodiscard]] bool FindAudio(const FString& FilePath, FMockAudioSerializedData& OutData) const noexcept
	{
		for (const auto& Audio : MockAudioList)
		{
			if (Audio.AudioSerializedData.FilePath == FilePath)
			{
				OutData = Audio;
				return true;
			}
		}

		return false;
	}

	using FMockAudioMapper = TFunction<FAudioSerializedData(const FMockAudioSerializedData&)>;
	
	[[nodiscard]] TArray<FAudioSerializedData> GetMockAudios() const noexcept
	{
		FMockAudioMapper Mapper = [](const FMockAudioSerializedData& Data)
		{
			return Data.AudioSerializedData;
		};
		return MockData::MapCopy(MockAudioList, Mapper);
	}
	
	[[nodiscard]] FAppSerializedData ToAppSerializeData() const noexcept 
	{
		FAppSerializedData Data(
			MockProjects
		);
		
		return Data;
	}
};
