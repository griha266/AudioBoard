#include "FilePicker.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"

FString SystemUtils::FFilePicker::OpenFileDialog() const noexcept
{
	TArray<FString> OutResults;
	bool IsOpened = false;
	if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
	{
		IsOpened = DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			TEXT("Open Audio Board project File"),
			DefaultFolderPath,
			TEXT(""),
			FileFilter,
			EFileDialogFlags::None,
			OutResults
		);
	}

	if (!IsOpened || OutResults.Num() == 0)
	{
		return TEXT("");
	}
	
	return OutResults[0];
}
