#pragma once
#include "MocksUtils.h"
#include "Models/AudioModel.h"
#include "Models/ProjectModel.h"

namespace ModelFactories
{
	static TObjectPtr<UAudioModel> AudioModelFactory(const FAudioSerializedData& AudioData, UObject* Outer)
	{
		const FAudioModelInitData AudioInitData(
			AudioData,
			&MocksUtils::LoadSoundWave
		);

		const auto AudioModel = NewObject<UAudioModel>(Outer);
		AudioModel->Init(AudioInitData);
		return ToObjectPtr(AudioModel);
	}

	static TObjectPtr<UProjectModel> ProjectModelFactory(const FString& Path, UObject* Outer)
	{
		const FProjectModelInitArgs ProjectInitArgs(
			Path,
			&AudioModelFactory,
			&MocksUtils::LoadMockProject
		);
		const auto ProjectModel = NewObject<UProjectModel>(Outer);
		ProjectModel->Initialize(ProjectInitArgs);
		return ToObjectPtr(ProjectModel);
	}
}
