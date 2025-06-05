#include "SystemUtils.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "LegacyScreenPercentageDriver.h"

#define LOCTEXT_NAMESPACE "FSystemUtilsModule"

void FSystemUtilsModule::StartupModule()
{
    
}

void FSystemUtilsModule::ShutdownModule()
{
    
}

// // TODO unreal has thread blocked version, we might want to change it
// TFuture<FString> FSystemUtilsModule::penFilePicker()
// {
// 	TPromise<FString> Operation;
// 	TFuture<FString> OperationResult;
//
// 	TArray<FString> OutFiles;
//
// 	bool bOpen = false;
// 	if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
// 	{
// 		const FString Filter = TEXT("*.audiobrd");
//
// 		// TODO recheck default paths and file name
// 		bOpen = DesktopPlatform->OpenFileDialog(
// 			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
// 			TEXT("Open audioboard project file"),
// 			TEXT(""),
// 			TEXT(""),
// 			*Filter,
// 			EFileDialogFlags::None,
// 			OutFiles
// 		);
// 	}
// 	if (!bOpen || OutFiles.Num() == 0)
// 	{
// 		Operation.SetValue(TEXT(""));
// 	} else
// 	{
// 		Operation.SetValue(OutFiles[0]);
// 	}
//
// 	return OperationResult;
// }

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSystemUtilsModule, SystemUtils)