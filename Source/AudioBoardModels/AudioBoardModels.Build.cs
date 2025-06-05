using UnrealBuildTool;

public class AudioBoardModels : ModuleRules
{
    public AudioBoardModels(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "InputCore",
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject",
                "Engine"
            ]
        );
    }
}