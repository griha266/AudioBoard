#pragma once
namespace SystemUtils
{
	class SYSTEMUTILS_API FFilePicker
	{
	public:
		explicit FFilePicker(const FString& DefaultFolderPath, const FString& Filter)
			:DefaultFolderPath(DefaultFolderPath),FileFilter(Filter)
		{ }
		
		[[nodiscard]] FString OpenFileDialog() const noexcept;
	private:
		const FString DefaultFolderPath;
		const FString FileFilter;
	};
}
