#pragma once


template<typename T>
concept initializable = requires(T Initializable)
{
	{ Initializable.GetIsInitialized() } -> std::same_as<bool>;
};
namespace AudioBoardUtils
{
	// TODO check unreal compile time reflection
	template<initializable T>
	[[nodiscard]] static bool IsReady(const T* const Model)
	{
		if (Model->GetIsInitialized())
		{
			return true;
		}
		UE_LOG(LogTemp, Error, TEXT("Model is not initialized"));
		return false;
	}


}


