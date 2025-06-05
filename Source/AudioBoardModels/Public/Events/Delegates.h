#pragma once
#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Data/ModelState.h"
#include "Delegates.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleDynamicMulticastDelegate);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModelStateChangedDelegate, EModelState, NewModelState);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModelBoolFieldChangedDelegate, bool, NewValue);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModelFloatFieldChangedDelegate, float, NewValue);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModelKeyFieldChangedDelegate, const FKey&, NewValue);


// TODO it should return to game thread by himself, as others expect this behaviour from him
template<typename T>
using TFileLoaderFunction = TFunction<TFuture<TTuple<bool, T>>(const FString&, TSharedPtr<FThreadSafeBool>)>;

using FFileOpenerFunction = TFunction<TFuture<FString>(TSharedPtr<FThreadSafeBool> CancellationToke)>;
