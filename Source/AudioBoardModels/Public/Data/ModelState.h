#pragma once

#include "CoreMinimal.h"
#include "ModelState.generated.h"


UENUM(BlueprintType)
enum class EModelState : uint8
{
	Unloaded,
	Loading,
	Loaded
};
