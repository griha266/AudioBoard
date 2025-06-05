#pragma once
#include "JsonObjectConverter.h"


class SYSTEMUTILS_API FFileLoader
{
public:
	template<typename TStruct>
	[[nodiscard]] static TFuture<TTuple<bool,TStruct>> LoadFileFromJson(const FString& Path, TSharedPtr<FThreadSafeBool> CT) noexcept;
	template<typename TStruct>
	[[nodiscard]] static TFuture<bool> SaveFileToJson(const FString& Path, const TStruct& Value, TSharedPtr<FThreadSafeBool> CT) noexcept;
};

// TODO we could use FOutputDeviceFile for making it async, but no need for now
template <typename TOutput>
TFuture<TTuple<bool, TOutput>> FFileLoader::LoadFileFromJson(const FString& Path, TSharedPtr<FThreadSafeBool> CT) noexcept
{
	TPromise<TTuple<bool, TOutput>> Operation;
	TFuture<TTuple<bool,TOutput>> OperationResult = Operation.GetFuture();
	TOutput Output;
	FString ResultJson;
	
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*Path))
	{
		Operation.SetValue(TTuple<bool,TOutput>(false, Output));
	}
	else if (!FFileHelper::LoadFileToString(ResultJson, *Path))
	{
		Operation.SetValue(TTuple<bool,TOutput>(false, Output));
	}
	else if (!FJsonObjectConverter::JsonObjectStringToUStruct<TOutput>(ResultJson, *Output))
	{
		Operation.SetValue(TTuple<bool,TOutput>(false, Output));
	}
	else
	{
		Operation.SetValue(TTuple<bool, TOutput>(true, Output));
	}


	return OperationResult;
}

template <typename TStruct>
TFuture<bool> FFileLoader::SaveFileToJson(const FString& Path, const TStruct& Value, TSharedPtr<FThreadSafeBool> CT) noexcept
{
	TPromise<bool> Operation;
	TFuture<bool> OperationResult = Operation.GetFuture();

	FString ResultJson;
	if (!FJsonObjectConverter::UStructToJsonObjectString<TStruct>(Value, ResultJson))
	{
		Operation.SetValue(false);
	}
	else if (!FFileHelper::SaveStringToFile(ResultJson, *Path))
	{
		Operation.SetValue(false);
	}
	else
	{
		Operation.SetValue(true);
	}

	return OperationResult;
}

